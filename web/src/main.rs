extern crate dekunobou;
use actix_cors::Cors;
use actix_files::Files;
use actix_web::{get, post, put, web, App, HttpResponse, HttpServer, Responder};
use dotenv::dotenv;
use log::{debug, error};
use serde::{Deserialize, Serialize};
use sqlx::postgres::PgPoolOptions;
use std::env;
use std::ffi::CString;
use std::os::raw::c_char;

#[derive(Deserialize)]
struct EngineOption {
    board: String,
    turn: u8,
    #[serde(default = "default_depth")]
    depth: u32,
    #[serde(default = "default_perfect_search_depth")]
    perfect_search_depth: u32,
}

#[derive(Debug, Deserialize, Clone)]
struct Config {
    username: String,
    password: String,
    dbname: String,
    tablename: String,
}

// ゲーム結果のリクエストボディ
#[derive(Debug, Deserialize)]
struct GameResult {
    winner: i32,
    ai_turn: i32,
    ai_level: i32,
    black_point: i32,
    white_point: i32,
}

// AIのレベルを取得するリクエストボディ
#[derive(Debug, Deserialize)]
struct AILevel {
    ai_level: i32,
}

// AIの対局結果のレスポンスボディ
#[derive(Debug, Serialize)]
struct AIResult {
    ai_win: i64,
    ai_lose: i64,
    draw: i64,
    win_rate: Option<String>,
}

fn default_depth() -> u32 {
    7
}

fn default_perfect_search_depth() -> u32 {
    13
}

// データベースへの接続プールを作成
async fn create_db_pool(config: &Config) -> sqlx::Result<sqlx::Pool<sqlx::Postgres>> {
    let database_url = format!(
        "postgres://{}:{}@db:5432/{}",
        config.username, config.password, config.dbname
    );
    PgPoolOptions::new()
        .max_connections(5)
        .connect(&database_url)
        .await
}

#[derive(Serialize)]
struct EngineResponse {
    r#move: u32,
}

#[put("/put")]
async fn put(engine_option: web::Json<EngineOption>) -> impl Responder {
    let board_string: CString = CString::new(engine_option.board.clone()).unwrap();
    let board_string_ptr: *const c_char = board_string.as_ptr();
    let ai_move;
    unsafe {
        ai_move = dekunobou::dekunobou(
            board_string_ptr,
            engine_option.turn != 0,
            engine_option.depth,
            engine_option.perfect_search_depth,
        );
    }

    web::Json(EngineResponse { r#move: ai_move })
}

// ゲーム結果を登録するハンドラー
#[post("/post")]
async fn register_result_handler(
    pool: web::Data<sqlx::Pool<sqlx::Postgres>>,
    json: web::Json<GameResult>,
    config: web::Data<Config>,
) -> impl Responder {
    match register_result(&pool, &json.into_inner(), &config).await {
        Ok(_) => HttpResponse::Ok().json(serde_json::json!({"status": "success"})),
        Err(e) => {
            error!("Error registering result: {}", e);
            HttpResponse::InternalServerError().json(serde_json::json!({
                "status": "error",
                "message": "Failed to insert into database"
            }))
        }
    }
}

// ゲーム結果をデータベースに登録
async fn register_result(
    pool: &sqlx::Pool<sqlx::Postgres>,
    game_result: &GameResult,
    config: &Config,
) -> sqlx::Result<()> {
    let GameResult {
        winner,
        ai_turn,
        ai_level,
        black_point,
        white_point,
    } = game_result;

    // デバッグ情報をログに記録
    debug!(
        "Received GameResult: winner={}, ai_turn={}, black_point={}, white_point={}",
        winner, ai_turn, black_point, white_point
    );

    // データベースに挿入
    let query = format!(
        "INSERT INTO {} (winner, ai_turn, ai_level, black_point, white_point) VALUES ($1, $2, $3, $4, $5)",
        config.tablename
    );

    sqlx::query(&query)
        .bind(winner)
        .bind(ai_turn)
        .bind(ai_level)
        .bind(black_point)
        .bind(white_point)
        .execute(pool)
        .await
        .map(|_| ())
}

// AIの対局結果を取得するハンドラー
#[get("/get_ai_result")]
async fn get_ai_result_handler(
    pool: web::Data<sqlx::Pool<sqlx::Postgres>>,
    config: web::Data<Config>,
) -> impl Responder {
    match get_ai_result(&pool, &config).await {
        Ok(result) => HttpResponse::Ok().json(result),
        Err(e) => {
            error!("Error fetching AI result: {}", e);
            HttpResponse::InternalServerError().json(serde_json::json!({
                "status": "error",
                "message": "Failed to fetch AI result"
            }))
        }
    }
}

// AIの対局結果をデータベースから取得
async fn get_ai_result(
    pool: &sqlx::Pool<sqlx::Postgres>,
    config: &Config,
) -> sqlx::Result<AIResult> {
    let tablename = &config.tablename;

    // クエリの定義
    let ai_win_query = format!("SELECT COUNT(*) FROM {} WHERE winner = ai_turn", tablename);
    let ai_lose_query = format!(
        "SELECT COUNT(*) FROM {} WHERE winner != ai_turn AND winner != 3",
        tablename
    );
    let draw_query = format!("SELECT COUNT(*) FROM {} WHERE winner = 3", tablename);

    // データの取得
    let ai_win: i64 = sqlx::query_scalar(&ai_win_query)
        .fetch_one(pool)
        .await
        .unwrap_or(0);

    let ai_lose: i64 = sqlx::query_scalar(&ai_lose_query)
        .fetch_one(pool)
        .await
        .unwrap_or(0);

    let draw: i64 = sqlx::query_scalar(&draw_query)
        .fetch_one(pool)
        .await
        .unwrap_or(0);

    // 勝率の計算
    let battle_all_count = ai_win + ai_lose + draw;
    let win_rate = if battle_all_count > 0 {
        let rate = (ai_win as f64 / battle_all_count as f64) * 100.0;
        Some(format!("{:.2}", rate))
    } else {
        None
    };

    Ok(AIResult {
        ai_win,
        ai_lose,
        draw,
        win_rate,
    })
}

// AIのレベルごとに対局結果を取得するハンドラー
#[post("/get_ai_result_by_level")]
async fn get_ai_result_by_level_handler(
    pool: web::Data<sqlx::Pool<sqlx::Postgres>>,
    json: web::Json<AILevel>,
    config: web::Data<Config>,
) -> impl Responder {
    match get_ai_result_by_level(&pool, &json.into_inner(), &config).await {
        Ok(result) => HttpResponse::Ok().json(result),
        Err(e) => {
            error!("Error fetching AI result: {}", e);
            HttpResponse::InternalServerError().json(serde_json::json!({
                "status": "error",
                "message": "Failed to fetch AI result"
            }))
        }
    }
}

// AIのレベルごとに対局結果をデータベースから取得
async fn get_ai_result_by_level(
    pool: &sqlx::Pool<sqlx::Postgres>,
    ai_level_struct: &AILevel,
    config: &Config,
) -> sqlx::Result<AIResult> {
    let AILevel {
        ai_level,
    } = ai_level_struct;
    let tablename = &config.tablename;

    debug!(
        "Received AILevel: ai_level={}",
        ai_level
    );

    // クエリの定義
    let ai_win_query = format!("SELECT COUNT(*) FROM {} WHERE winner = ai_turn AND ai_level = {}", tablename, ai_level);
    let ai_lose_query = format!(
        "SELECT COUNT(*) FROM {} WHERE winner != ai_turn AND winner != 3 AND ai_level = {}",
        tablename, ai_level
    );
    let draw_query = format!("SELECT COUNT(*) FROM {} WHERE winner = 3 AND ai_level = {}", tablename, ai_level);

    // データの取得
    let ai_win: i64 = sqlx::query_scalar(&ai_win_query)
        .fetch_one(pool)
        .await
        .unwrap_or(0);

    let ai_lose: i64 = sqlx::query_scalar(&ai_lose_query)
        .fetch_one(pool)
        .await
        .unwrap_or(0);

    let draw: i64 = sqlx::query_scalar(&draw_query)
        .fetch_one(pool)
        .await
        .unwrap_or(0);

    // 勝率の計算
    let battle_all_count = ai_win + ai_lose + draw;
    let win_rate = if battle_all_count > 0 {
        let rate = (ai_win as f64 / battle_all_count as f64) * 100.0;
        Some(format!("{:.2}", rate))
    } else {
        None
    };

    Ok(AIResult {
        ai_win,
        ai_lose,
        draw,
        win_rate,
    })
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    // ログを初期化
    env_logger::init();

    // 環境変数を読み込む
    dotenv().ok();

    // Configの読み込み
    let config = Config {
        username: env::var("DB_USERNAME").expect("DB_USERNAME is not set"),
        password: env::var("DB_PASSWORD").expect("DB_PASSWORD is not set"),
        dbname: env::var("DB_NAME").expect("DB_NAME is not set"),
        tablename: env::var("TABLE_NAME").expect("TABLE_NAME is not set"),
    };

    // データベースプールの作成
    let pool = create_db_pool(&config)
        .await
        .expect("Failed to connect to the database");
    let pool_data = web::Data::new(pool);

    let config_data = web::Data::new(config.clone());

    let port = env::var("PORT")
        .unwrap_or_else(|_| "5000".to_string())
        .parse()
        .expect("PORT must be a number");

    HttpServer::new(move || {
        let cors = Cors::default()
            .allow_any_origin()
            .allow_any_method()
            .allow_any_header();

        App::new()
            .wrap(cors)
            .app_data(pool_data.clone())
            .app_data(config_data.clone())
            .service(put)
            .service(register_result_handler)
            .service(get_ai_result_handler)
            .service(get_ai_result_by_level_handler)
            .service(Files::new("/", "./static").index_file("index.html"))
    })
    .bind(("0.0.0.0", port))?
    .run()
    .await
}
