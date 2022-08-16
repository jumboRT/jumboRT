pub mod server;
pub mod client;
pub mod ser;

fn main() -> std::io::Result<()> {
    let server = server::Server::new("0.0.0.0:29300")?;
    server.run()?;
    Ok(())
}
