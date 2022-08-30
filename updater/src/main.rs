use std::{env, fs};
use std::path::Path;

const repo_dir: &str = ".miniRT";

fn has_repo() -> std::io::Result<()> {
    let mut path = env::current_dir()?;
    path.push(repo_dir);
    let attr = match fs::metadata(path)?;
    Ok(attr.is_dir())
}

fn main() -> std::io::Result<()> {
    println!("Hello, world!");
    let mut path = env::current_dir()?;
    path.push(".git/");
    let exists = match fs::metadata(path) {
	Ok(..) => true,
	Err(..) => false,
    };
    println!("git dir exists? {}", exists);

    let mut builder = git2::build::RepoBuilder::new();

    match builder.clone(
	"https://github.com/Hyxogen/Philosophers",
	Path::new(".philo"),
    ) {
	Err(err) => println!("failed to clone repo {}", err),
	Ok(..) => println!("cloned repo!"),
    };
    Ok(())
}
