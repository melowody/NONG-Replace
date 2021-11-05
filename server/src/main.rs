#![feature(decl_macro)]

#[macro_use]
extern crate rocket;
extern crate reqwest;
extern crate dotenv;

mod utils;

use mysql::*;
use mysql::prelude::*;

use rocket_contrib::{json::{Json}};

pub fn download_file(url: String) -> Vec<u8> {

    let client = reqwest::blocking::Client::new();
    let resp = client.get(url).send().unwrap();
    assert!(resp.status().is_success());
    resp.bytes().unwrap().as_ref().to_vec()
}

pub fn get_link(id: i32) -> String {

    let mut row: Row = match utils::sql::CONN.lock().unwrap().exec_first("SELECT Link FROM Songs WHERE ID=:level_id;",
        mysql::params! {
            "level_id" => id
        }
    ).unwrap() {
        Some(r) => r,
        None => {
            return "".to_string();
        }
    };

    row.take("Link").unwrap()

}

#[get("/levels")]
pub fn get_levels() -> Json<Vec<i32>> {
    let levels: Vec<i32> = utils::sql::CONN.lock().unwrap().query("SELECT ID FROM Songs").unwrap();
    Json(levels)
}

#[get("/<id>")]
pub fn get_file(id: i32) -> Vec<u8> {
    let link = get_link(id);
    if !link.is_empty() {
        download_file(link)
    } else {
        vec![]
    }
}

fn main() {
    rocket::ignite()
        .mount("/",
        routes![
            get_file,
            get_levels
        ],
    ).launch();
}
