extern crate dekunobou_lib;
use std::ffi::CString;
use std::os::raw::c_char;
use actix_web::{get, put, web, App, HttpServer, Responder};
use actix_cors::Cors;
use serde::{Deserialize, Serialize};

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
    let board_string_ptr: *const c_char = board_string.as_ptr();
    let ai_move;
    unsafe {
        ai_move = dekunobou_lib::dekunobou(board_string_ptr, engine_option.turn != 0, engine_option.depth, engine_option.perfect_search_depth);
    }

    web::Json(EngineResponse {r#move: ai_move})
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| {
        let cors = Cors::default()
            .allow_any_origin()
            .allow_any_method()
            .allow_any_header();

        App::new()
            .wrap(cors)
            .service(index)
            .service(put)
    })
    .bind(("0.0.0.0", 5000))?
    .run()
    .await
}