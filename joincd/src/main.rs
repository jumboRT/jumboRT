pub mod client;
pub mod ser;
pub mod server;

pub type DynResult<T> = Result<T, Box<dyn std::error::Error>>;

fn main() -> DynResult<()> {
    let server = server::Server::new("0.0.0.0:29300")?;
    server.run()?;
    Ok(())
}
