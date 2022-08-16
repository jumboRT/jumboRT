use std::net::{TcpListener, ToSocketAddrs};
use std::io;
use std::sync::{Arc, RwLock};
use std::thread;
use std::sync::atomic::AtomicU64;
use crate::client::Client;

pub struct Work {
    pub begin: u64,
    pub end: u64,
}

pub struct Job {
    pub id: u64,
    pub scene: String,
    pub work: Vec<Work>,
}

pub struct Server {
    pub listener: TcpListener,
    pub clients: RwLock<Vec<Arc<Client>>>,
    pub job_seq: AtomicU64,
    pub jobs: RwLock<Vec<Job>>,
}

impl Server {
    pub fn new<A: ToSocketAddrs>(addr: A) -> io::Result<Arc<Server>> {
        let listener = TcpListener::bind(addr)?;

        Ok(Arc::new(Server {
            listener,
            clients: RwLock::new(Vec::new()),
            job_seq: AtomicU64::new(0),
            jobs: RwLock::new(Vec::new()),
        }))
    }

    pub fn run(self: Arc<Self>) -> io::Result<()> {
        for stream in self.listener.incoming() {
            let client = Client::new(Arc::downgrade(&self), stream?)?;

            {
                let mut clients = self.clients.write().unwrap();
                clients.push(client.clone());
            }

            thread::spawn(move || client.run().unwrap());
        }

        Ok(())
    }

    pub fn update_jobs(&self) -> io::Result<()> {
        Ok(())
    }

    pub fn update_work(&self) -> io::Result<()> {
        Ok(())
    }
}
