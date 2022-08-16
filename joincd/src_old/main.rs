pub mod ser;

use std::net::{TcpListener, TcpStream, SocketAddr};
use std::io::{self, Read, Write};
use ser::*;
use std::thread;
use std::time::Duration;

pub struct Client {
    stream: TcpStream,
    addr: SocketAddr,
    reader: Vec<u8>,
    writer: Vec<u8>,
    closing: bool,
}

pub struct Server {
    clients: Vec<Client>,
    listener: TcpListener,
}

pub fn poll<T, F: FnMut() -> io::Result<T>>(mut f: F) -> Option<io::Result<T>> {
    match f() {
        Ok(n) => Some(Ok(n)),
        Err(ref e) if e.kind() == io::ErrorKind::WouldBlock => None,
        Err(ref e) if e.kind() == io::ErrorKind::Interrupted => poll(f),
        Err(e) => Some(Err(e)),
    }
}

impl Client {
    pub fn new(stream: TcpStream) -> io::Result<Client> {
        stream.set_nonblocking(true)?;
        let addr = stream.peer_addr()?;
        Ok(Client {
            stream,
            addr,
            reader: Vec::new(),
            writer: Vec::new(),
            closing: false,
        })
    }

    pub fn poll_write(&mut self) -> io::Result<()> {
        if self.writer.len() != 0 {
            while let Some(n) = poll(|| self.stream.write(&self.writer)) {
                self.writer.drain(0..n?);
            }
        }
        Ok(())
    }

    pub fn poll_read(&mut self) -> io::Result<()> {
        let mut buf = [0; 4096];
        while let Some(n) = poll(|| self.stream.read(&mut buf)) {
            match n? {
                0 => { self.closing = true; break },
                n => self.reader.extend(&buf[..n]),
            }
        }
        Ok(())
    }

    pub fn poll(&mut self) -> io::Result<()> {
        self.poll_write()?;
        self.poll_read()?;
        while self.reader.len() >= 9 {
            let length = read_u64(&self.reader, 0) as usize;
            let packet_id = read_u8(&self.reader, 8);
            if self.reader.len() < length + 9 {
                break
            }
            let data = self.reader[9..length + 9].to_vec();
            self.reader.drain(0..length + 9);
            self.handle_packet(packet_id, &data)?;
        }
        Ok(())
    }

    pub fn handle_echo_packet(&mut self, data: &[u8]) {
        println!("[{}] echo: {:?}", self.addr, std::str::from_utf8(data));
    }

    pub fn handle_invalid_packet(&mut self, data: &[u8]) {
        println!("[{}] invalid packet: {:?}", self.addr, data);
    }

    pub fn handle_packet(&mut self, packet_id: u8, data: &[u8]) -> io::Result<()> {
        match packet_id {
            0 => Ok(self.handle_echo_packet(data)),
            _ => Ok(self.handle_invalid_packet(data)),
        }
    }
}

impl Server {
    pub fn new() -> io::Result<Server> {
        let listener = TcpListener::bind("0.0.0.0:29300")?;
        listener.set_nonblocking(true)?;
        Ok(Server {
            clients: Vec::new(),
            listener,
        })
    }

    pub fn poll_accept(&mut self) -> io::Result<()> {
        while let Some(stream) = poll(|| self.listener.accept()) {
            let (stream, addr) = stream?;
            println!("connection ({}) from {}", self.clients.len(), addr);
            self.clients.push(Client::new(stream)?);
        }
        Ok(())
    }

    pub fn poll(&mut self) -> io::Result<()> {
        self.poll_accept()?;
        for client in self.clients.iter_mut() {
            client.poll()?;
        }
        self.clients.retain(|c| !c.closing);
        Ok(())
    }
}

fn main() -> io::Result<()> {
    let mut server = Server::new()?;

    loop {
        server.poll()?;
        thread::sleep(Duration::from_millis(1));
    }
}
