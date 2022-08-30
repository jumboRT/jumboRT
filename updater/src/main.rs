use crossterm::{cursor, QueueableCommand};
use git2::Repository;
use regex::Regex;
use std::fs;
use std::io::{stdin, stdout, Write};
use std::path::Path;
use std::process::Command;

type DynResult<T> = Result<T, Box<dyn std::error::Error>>;

const REPO_DIR: &str = ".miniRT/";
const REPO_URL: &str = "https://github.com/DutChen18/42-miniRT.git";
const DEFAULT_HOST: &str = "localhost";
const DEFAULT_PORT: &str = "29300";
const KERNELS_FILE: &str = "kernels.txt";
const ARGS_FILE: &str = "args.txt";

#[cfg(windows)]
mod config {
    pub const REPO_BRANCH: &str = "windows";
    pub const COMPILER_EXEC: &str = "compile.exe";
    pub const RT_EXEC: &str = "miniRT.exe";
}

#[cfg(not(windows))]
mod config {
    pub const REPO_BRANCH: &str = "unix";
    pub const COMPILER_EXEC: &str = "compile";
    pub const RT_EXEC: &str = "miniRT";
}

use config::*;

fn get_repo() -> DynResult<Repository> {
    Ok(Repository::open(REPO_DIR)?)
}

fn fast_forward_repo(repo: &Repository, commit: git2::AnnotatedCommit) -> DynResult<()> {
    let refname = format!("refs/heads{}", REPO_BRANCH);
    let mut reference = repo.find_reference(&refname)?;
    reference.set_target(commit.id(), "Fast-Forward")?;
    repo.set_head(&refname)?;
    repo.checkout_head(Some(git2::build::CheckoutBuilder::default().force()))?;
    Ok(())
}

fn transfer_progress_callback() -> git2::RemoteCallbacks<'static> {
    let mut callback = git2::RemoteCallbacks::new();

    let mut done = false;
    callback.transfer_progress(move |stats| {
        if stats.received_objects() == stats.total_objects() {
            if !done {
                println!();
                done = true;
            }
            stdout().queue(cursor::SavePosition).unwrap();
            print!(
                "Resolving deltas {}/{}",
                stats.indexed_deltas(),
                stats.total_deltas()
            );
            stdout().queue(cursor::RestorePosition).unwrap();
        } else if stats.total_objects() > 0 {
            stdout().queue(cursor::SavePosition).unwrap();
            print!(
                "Received {}/{} objects ({}) in {} bytes",
                stats.received_objects(),
                stats.total_objects(),
                stats.indexed_objects(),
                stats.received_bytes(),
            );
            stdout().queue(cursor::RestorePosition).unwrap();
        }
        stdout().flush().unwrap();
        true
    });
    callback
}

fn fetch_repo(repo: &Repository) -> DynResult<()> {
    let callback = transfer_progress_callback();

    let mut options = git2::FetchOptions::new();
    options.remote_callbacks(callback);
    repo.find_remote("origin")?
        .fetch(&[REPO_BRANCH], Some(&mut options), None)?;
    Ok(())
}

fn update_repo(repo: Repository) -> DynResult<()> {
    fetch_repo(&repo)?;
    let fetch_head = repo.find_reference("FETCH_HEAD")?;
    let fetch_commit = repo.reference_to_annotated_commit(&fetch_head)?;
    let analysis = repo.merge_analysis(&[&fetch_commit])?;

    match analysis.0 {
        a if a.is_up_to_date() => Ok(()),
        a if a.is_fast_forward() => Ok(fast_forward_repo(&repo, fetch_commit)?),
        _ => Err("an unknown error occurred".into()),
    }
}

fn clone_repo() -> DynResult<Repository> {
    let callback = transfer_progress_callback();

    let mut options = git2::FetchOptions::new();
    options.remote_callbacks(callback);

    let mut builder = git2::build::RepoBuilder::new();
    builder.fetch_options(options);
    builder.branch(REPO_BRANCH);
    let repo = builder.clone(REPO_URL, Path::new(REPO_DIR))?;
    stdout().write_all("\n".as_bytes()).unwrap();
    stdout().flush().unwrap();
    Ok(repo)
}

fn reset_repo(repo: &Repository) -> DynResult<()> {
    let head = repo.find_reference("HEAD")?;
    let object = head.peel(git2::ObjectType::Commit)?;
    repo.reset(&object, git2::ResetType::Hard, None)?;
    Ok(())
}

fn get_host(stdin: &std::io::Stdin) -> DynResult<String> {
    print!("enter hostname (enter nothing for: {}): ", DEFAULT_HOST);
    stdout().flush().unwrap();
    let mut buffer = String::new();
    stdin.read_line(&mut buffer)?;

    match buffer.trim() {
        b if b.is_empty() => Ok(DEFAULT_HOST.to_string()),
        b => Ok(b.to_string()),
    }
}

fn get_port(stdin: &std::io::Stdin) -> DynResult<String> {
    print!("enter port (enter nothing for: {}): ", DEFAULT_PORT);
    stdout().flush().unwrap();
    let mut buffer = String::new();
    stdin.read_line(&mut buffer)?;

    match buffer.trim() {
        b if b.is_empty() => Ok(DEFAULT_PORT.to_string()),
        b => Ok(b.to_string()),
    }
}

fn compile_kernels() -> DynResult<()> {
    let cwd = std::env::current_dir()?;
    let files = fs::read_dir(cwd)?;
    let re = Regex::new(r".+\.bin").unwrap();

    for file in files {
        if re.is_match(&file.as_ref().unwrap().file_name().into_string().unwrap()) {
            fs::remove_file(file.unwrap().path())?;
        }
    }

    let mut cwd = std::env::current_dir()?;
    cwd.push(REPO_DIR);
    cwd.push(KERNELS_FILE);
    let kernels_file = fs::read_to_string(cwd.as_path())?;
    println!("{}", kernels_file);
    loop {
        let status = Command::new(fs::canonicalize(format!("{}/{}", REPO_DIR, COMPILER_EXEC))?)
            .args(kernels_file.split_whitespace())
            .current_dir(REPO_DIR)
            .status().unwrap();

        if status.success() {
            break Ok(());
        }
    }
}

fn run_program(host: &str, port: &str) -> DynResult<bool> {
    let mut dir = std::env::current_dir()?;
    dir.push(REPO_DIR);
    dir.push(ARGS_FILE);
    let args = fs::read_to_string(dir.as_path()).unwrap();
    Ok(Command::new(fs::canonicalize(format!("{}/{}", REPO_DIR, RT_EXEC))?)
        .args(args.replace("HOST", host).replace("PORT", port).split_whitespace())
        .current_dir(REPO_DIR)
        .status().unwrap()
        .success())
}

fn update_and_run(host: String, port: String) -> DynResult<()> {
    loop {
        let repo = get_repo().or_else(|_| clone_repo())?;
        reset_repo(&repo)?;
        update_repo(repo)?;
        compile_kernels()?;
        if !(run_program(&host, &port)?) {
            break Ok(());
        }
    }
}

fn main() -> DynResult<()> {
    let stdin = stdin();
    let host = get_host(&stdin)?;
    let port = get_port(&stdin)?;
    update_and_run(host, port)
}
