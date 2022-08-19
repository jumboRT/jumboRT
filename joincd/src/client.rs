use std::net::{TcpStream, SocketAddr};
use std::sync::{Arc, Weak, RwLock, Mutex};
use std::sync::atomic::{AtomicU64, Ordering};
use std::io::{self, Read, Write};
use crate::ser;
use crate::server::{Server, Job, Work};

const VERBOSE: bool = false;

pub enum ClientState {
    Unknown,
    Worker,
    Viewer,
}

pub struct Client {
    pub server: Weak<Server>,
    pub addr: SocketAddr,
    pub stream: TcpStream,
    pub mutex: Mutex<()>,
    pub state: RwLock<ClientState>,
    pub job_id: RwLock<Option<u64>>,
    pub work: AtomicU64,
}

impl Client {
    pub fn new(server: Weak<Server>, stream: TcpStream) -> io::Result<Arc<Client>> {
        let addr = stream.peer_addr()?;
        println!("[{}] connected", addr);

        Ok(Arc::new(Client {
            server,
            stream,
            addr,
            mutex: Mutex::new(()),
            state: RwLock::new(ClientState::Unknown),
            job_id: RwLock::new(None),
            work: AtomicU64::new(0),
        }))
    }

    pub fn cancel_job(&self) -> io::Result<()> {
        let server = self.server.upgrade().unwrap();

        {
            let mut job_id_opt = self.job_id.write().unwrap();
            let mut server_jobs = server.jobs.write().unwrap();

            if let Some(job_id) = *job_id_opt {
                *job_id_opt = None;
                server_jobs.retain(|j| j.id != job_id);
            }
        }
        
        server.update_jobs()
    }

    pub fn update_job_id(&self) -> io::Result<u64> {
        self.cancel_job()?;
        let server = self.server.upgrade().unwrap();
        let job_id = server.job_seq.fetch_add(1, Ordering::SeqCst);
        let mut job_id_opt = self.job_id.write().unwrap();
        *job_id_opt = Some(job_id);
        Ok(job_id)
    }

    pub fn send_job(&self, job: &Job) -> io::Result<()> {
        let mut job_id_opt = self.job_id.write().unwrap();
        let mut packet = Vec::new();
        let scene_name = job.scene.as_bytes();
        
        if *job_id_opt != Some(job.id) {
            println!("[{}] send job id={} scene={}", self.addr, job.id, job.scene);
            *job_id_opt = Some(job.id);
            self.work.store(4, Ordering::SeqCst);
            ser::write_u64(&mut packet, job.id);
            ser::write_u64(&mut packet, job.width);
            ser::write_u64(&mut packet, job.height);
            ser::write_u64(&mut packet, scene_name.len() as u64);
            packet.extend(scene_name);
            self.write_packet(3, &packet)?;
        }

        Ok(())
    }

    pub fn send_work(&self, job: &mut Job) -> io::Result<()> {
        let job_id_opt = self.job_id.read().unwrap();
        let w = job.work.len() as u64;

        if *job_id_opt == Some(job.id) {
            if let Ok(p) = self.work.fetch_update(Ordering::SeqCst, Ordering::SeqCst, |v| Some(v.max(w) - w)) {
                let n = p.min(w) as usize;

                for work in job.work[0..n].iter() {
                    if VERBOSE {
                        println!("[{}] send work begin={} end={}", self.addr, work.begin, work.end);
                    }

                    let mut packet = Vec::new();

                    ser::write_u64(&mut packet, work.begin);
                    ser::write_u64(&mut packet, work.end);
                    self.write_packet(4, &packet)?;
                }

                job.work.drain(0..n);
            }
        }

        Ok(())
    }

    pub fn write_packet(&self, packet_id: u8, data: &[u8]) -> io::Result<()> {
        let mut packet = Vec::new();

        if VERBOSE {
            println!("[{}] send packet id={} len={}", self.addr, packet_id, data.len());
        }

        ser::write_u64(&mut packet, data.len() as u64);
        ser::write_u8(&mut packet, packet_id);
        packet.extend(data);

        let _lock = self.mutex.lock();
        (&self.stream).write_all(&packet)
    }

    pub fn handle_handshake_packet(&self, data: &[u8]) -> io::Result<()> {
        let new_state = ser::read_u8(&data[0..1]);
        let server = self.server.upgrade().unwrap();

        {
            let new_state = match new_state {
                0 => {
                    self.write_packet(0, &[])?;
                    println!("[{}] handshake worker", self.addr);
                    ClientState::Worker
                },
                1 => {
                    let mut packet = Vec::new();
                    let job_id = self.update_job_id()?;
                    ser::write_u64(&mut packet, job_id);
                    self.write_packet(0, &packet)?;
                    println!("[{}] handshake viewer", self.addr);
                    ClientState::Viewer
                },
                x => {
                    println!("[{}] handshake unknown({})", self.addr, x);
                    ClientState::Unknown
                },
            };
            
            let mut state = self.state.write().unwrap();
            *state = new_state;
        }
        
        server.update_jobs()
    }

    pub fn handle_ping_packet(&self, data: &[u8]) -> io::Result<()> {
        println!("[{}] ping", self.addr);
        self.write_packet(2, data)
    }

    pub fn handle_job_request_packet(&self, data: &[u8]) -> io::Result<()> {
        let width = ser::read_u64(&data[0..8]);
        let height = ser::read_u64(&data[8..16]);
        let length = ser::read_u64(&data[16..24]) as usize;
        let scene = std::str::from_utf8(&data[24..length + 24]).unwrap().to_string();
        let server = self.server.upgrade().unwrap();

        {
            let mut server_jobs = server.jobs.write().unwrap();
            let job_id_opt = self.job_id.read().unwrap();
            let job_id = job_id_opt.unwrap();
            println!("[{}] recv job id={} scene={}", self.addr, job_id, scene);

            server_jobs.push(Job {
                id: job_id,
                width,
                height,
                scene,
                work: Vec::new(),
            });
        }

        server.update_jobs()
    }

    pub fn handle_send_work_packet(&self, data: &[u8]) -> io::Result<()> {
        let begin = ser::read_u64(&data[0..8]);
        let end = ser::read_u64(&data[8..16]);
        let server = self.server.upgrade().unwrap();

        if VERBOSE {
            println!("[{}] recv work begin={} end={}", self.addr, begin, end);
        }

        {
            let mut server_jobs = server.jobs.write().unwrap();
            let job_id_opt = self.job_id.read().unwrap();
            let job_id = job_id_opt.unwrap();
            let job = server_jobs.iter_mut().find(|j| j.id == job_id);

            if let Some(job) = job {
                job.work.push(Work { begin, end });
            }
        }

        server.update_work()
    }

    pub fn handle_send_results_packet(&self, data: &[u8]) -> io::Result<()> {
        let job_id = ser::read_u64(&data[0..8]);
        let count = ser::read_u64(&data[8..16]) as usize;
        let results = &data[16..count * 24 + 16];
        let server = self.server.upgrade().unwrap();

        if VERBOSE {
            println!("[{}] results id={} count={}", self.addr, job_id, count);
        }

        {
            let clients = server.clients.read().unwrap();
            let job_id_opt = self.job_id.read().unwrap();

            let client = clients.iter().find(|c| {
                let client_job_id_opt = c.job_id.read().unwrap();
                let client_state = c.state.read().unwrap();

                match *client_state {
                    ClientState::Viewer => *client_job_id_opt == Some(job_id),
                    _ => false,
                }
            });

            if let Some(client) = client {
                let mut packet = Vec::new();

                ser::write_u64(&mut packet, job_id);
                ser::write_u64(&mut packet, count as u64);
                packet.extend(results);
                client.write_packet(5, &packet)?;

                if *job_id_opt == Some(job_id) {
                    self.work.fetch_add(1, Ordering::SeqCst);
                }
            }
        }

        server.update_work()
    }

    pub fn handle_log_packet(&self, data: &[u8]) -> io::Result<()> {
        let length = ser::read_u64(&data[0..8]) as usize;
        let string = std::str::from_utf8(&data[8..length + 8]).unwrap();
        println!("[{}] log {:?}", self.addr, string);

        Ok(())
    }

    pub fn handle_packet(&self, packet_id: u8, data: &[u8]) -> io::Result<()> {
        if VERBOSE {
            println!("[{}] recv packet id={} len={}", self.addr, packet_id, data.len());
        }

        match packet_id {
            0x00 => self.handle_handshake_packet(data),
            0x01 => self.handle_ping_packet(data),
            0x03 => self.handle_job_request_packet(data),
            0x04 => self.handle_send_work_packet(data),
            0x05 => self.handle_send_results_packet(data),
            0xFF => self.handle_log_packet(data),
            _ => Ok(())
        }
    }

    pub fn run_int(&self) -> io::Result<()> {
        let mut packet_buffer = Vec::new();
        let mut read_buffer = [0; 4096];

        loop {
            let count = (&self.stream).read(&mut read_buffer)?;
            packet_buffer.extend(&read_buffer[..count]);

            if count <= 0 {
                return Ok(());
            }

            while packet_buffer.len() >= 9 {
                let length = ser::read_u64(&packet_buffer[0..8]) as usize;
                let packet_id = ser::read_u8(&packet_buffer[8..9]);

                if packet_buffer.len() < 9 + length {
                    break
                }

                self.handle_packet(packet_id, &packet_buffer[9..length + 9])?;
                packet_buffer.drain(0..length + 9);
            }
        }
    }

    pub fn run(self: Arc<Self>) {
        let mut result = self.run_int();
        let server = self.server.upgrade().unwrap();
        let state = self.state.read().unwrap();

        {
            let mut clients = server.clients.write().unwrap();
            clients.retain(|c| Arc::as_ptr(c) != Arc::as_ptr(&self));
        }

        if let ClientState::Viewer = *state {
            result = result.and(self.cancel_job());
        }

        if let Err(err) = result {
            eprintln!("[{}] {}", self.addr, err);
        }

        println!("[{}] disconnected", self.addr);
    }
}
