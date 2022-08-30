use crossterm::{cursor, QueueableCommand};
use git2::Repository;
use regex::Regex;
use std::fs;
use std::io::{stdin, stdout, Write};
use std::path::Path;
use std::process::Command;

const REPO_DIR: &str = ".miniRT/";
const REPO_URL: &str = "https://github.com/Hyxogen/miniRT.git";
const DEFAULT_HOST: &str = "localhost";
const DEFAULT_PORT: &str = "29300";
const KERNELS_FILE: &str = "kernels.txt";
const ARGS_FILE: &str = "args.txt";
const REPO_BRANCH: &str = if cfg!(target_os = "windows") {
    "windows"
} else {
    "unix"
};
const COMPILER_EXEC: &str = if cfg!(target_os = "windows") {
    "compile.exe"
} else {
    "compile"
};
const RT_EXEC: &str = if cfg!(target_os = "windows") {
    "miniRT.exe"
} else {
    "miniRT"
};

fn get_repo() -> Result<Repository, git2::Error> {
    Repository::open(REPO_DIR)
}

fn fast_forward_repo(repo: &Repository, commit: git2::AnnotatedCommit) -> Result<(), git2::Error> {
    let refname = format!("refs/heads{}", REPO_BRANCH);
    let mut reference = repo.find_reference(&refname)?;
    reference.set_target(commit.id(), "Fast-Forward")?;
    repo.set_head(&refname)?;
    repo.checkout_head(Some(git2::build::CheckoutBuilder::default().force()))
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

fn fetch_repo(repo: &Repository) -> Result<(), Box<dyn std::error::Error>> {
    let callback = transfer_progress_callback();

    let mut options = git2::FetchOptions::new();
    options.remote_callbacks(callback);
    repo.find_remote("origin")?
        .fetch(&[REPO_BRANCH], Some(&mut options), None)?;
    Ok(())
}

fn update_repo(repo: Repository) -> Result<(), Box<dyn std::error::Error>> {
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

fn clone_repo() -> Result<Repository, git2::Error> {
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

fn reset_repo(repo: &Repository) -> Result<(), git2::Error> {
    let head = repo.find_reference("HEAD")?;
    let object = head.peel(git2::ObjectType::Commit)?;
    repo.reset(&object, git2::ResetType::Hard, None)?;
    Ok(())
}

fn get_host(stdin: &std::io::Stdin) -> Result<String, Box<dyn std::error::Error>> {
    print!("enter hostname (enter nothing for: {}): ", DEFAULT_HOST);
    stdout().flush().unwrap();
    let mut buffer = String::new();
    stdin.read_line(&mut buffer)?;

    if buffer.trim().is_empty() {
        Ok(DEFAULT_HOST.to_string())
    } else {
        Ok(buffer)
    }
}

fn get_port(stdin: &std::io::Stdin) -> Result<String, Box<dyn std::error::Error>> {
    print!("enter port (enter nothing for: {}): ", DEFAULT_PORT);
    stdout().flush().unwrap();
    let mut buffer = String::new();
    stdin.read_line(&mut buffer)?;

    if buffer.trim().is_empty() {
        Ok(DEFAULT_PORT.to_string())
    } else {
        Ok(buffer)
    }
}

fn compile_kernels() -> Result<(), Box<dyn std::error::Error>> {
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
    loop {
        let status = Command::new(COMPILER_EXEC)
            .args(kernels_file.split_whitespace())
            .status()?;

        if status.success() {
            break Ok(());
        }
    }
}

fn run_program(host: &str, port: &str) -> Result<bool, Box<dyn std::error::Error>> {
    let mut dir = std::env::current_dir()?;
    dir.push(REPO_DIR);
    dir.push(ARGS_FILE);
    let args = fs::read_to_string(dir.as_path())?;
    Ok(Command::new(RT_EXEC)
        .args(args.replace("HOST", host).replace("PORT", port).split_whitespace())
        .status()?
        .success())
}

fn update_and_run(host: String, port: String) -> Result<(), Box<dyn std::error::Error>> {
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

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let stdin = stdin();
    let host = get_host(&stdin)?;
    let port = get_port(&stdin)?;
    update_and_run(host, port)
}
