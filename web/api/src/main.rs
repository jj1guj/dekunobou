extern crate dekunobou;
use actix_cors::Cors;
use actix_web::{get, put, web, App, HttpServer, Responder};
use serde::{Deserialize, Serialize};
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

fn default_depth() -> u32 {
    7
}

fn default_perfect_search_depth() -> u32 {
    13
}

#[get("/")]
async fn index() -> impl Responder {
    "I'm dekunobou, computer othello program!\n"
}

#[derive(Serialize)]
struct EngineResponse {
    r#move: u32,
}

#[put("/put")]
async fn put(engine_option: web::Json<EngineOption>) -> impl Responder {
    let board_string: CString = CString::new(engine_option.board.clone()).unwrap();
    let turn = engine_option.turn != 0;
    let depth = engine_option.depth;
    let perfect_search_depth = engine_option.perfect_search_depth;

    let ai_move = tokio::task::spawn_blocking(move || {
        // Ensure `board_string` lives inside the closure to keep the pointer valid
        let board_string_ptr: *const c_char = board_string.as_ptr();
        unsafe {
            // Call the unsafe function within the blocking thread
            dekunobou::dekunobou(board_string_ptr, turn, depth, perfect_search_depth)
        }
    })
    .await
    .expect("Failed to execute blocking task");

    web::Json(EngineResponse { r#move: ai_move })
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    let port = env::var("PORT")
        .unwrap_or_else(|_| "5000".to_string())
        .parse()
        .expect("PORT must be a number");

    HttpServer::new(|| {
        let cors = Cors::default()
            .allow_any_origin()
            .allow_any_method()
            .allow_any_header();

        App::new().wrap(cors).service(index).service(put)
    })
    .bind(("0.0.0.0", port))?
    .run()
    .await
}
