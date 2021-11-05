use mysql::*;
use lazy_static::lazy_static;
use std::sync::Mutex;
use std::env;

lazy_static! {
    pub static ref CONN: Mutex<PooledConn> = {

        // Create a connection to the mysql database
        let username = env::var("SQL_USER").expect("SQL_USER is not set!");
        let password = env::var("SQL_PASS").expect("SQL_PASS is not set!");
        let port = env::var("SQL_PORT").expect("SQL_PORT is not set!");
        let url = format!("mysql://{}:{}@localhost:{}/NONG", username, password, port);
        let pool: Pool = Pool::new(url).unwrap();
        let connection: PooledConn = pool.get_conn().unwrap();

        Mutex::new(connection)
    };
}
