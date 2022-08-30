use crate::ser;
use crate::server::{Job, Server, Work};
use crate::DynResult;
use std::io::{Read, Write};
use std::net::{SocketAddr, TcpStream};
use std::sync::atomic::{AtomicU64, Ordering};
use std::sync::{Arc, Mutex, RwLock, Weak};

const VERBOSE: bool = false;
const PROTOVER: u64 = 1;

pub enum ClientState {
    Unknown,
    Worker,
    Viewer,
}

pub struct ResultItem {
    pub begin: u64,
    pub end: u64,
    pub data: Vec<u8>,
}

pub struct WorkItem {
    pub work: Work,
    pub finished_business: Vec<ResultItem>,
    pub totally_finished_business: u64,
}

pub struct Client {
    pub server: Weak<Server>,
    pub addr: SocketAddr,
    pub stream: TcpStream,
    pub mutex: Mutex<()>,
    pub state: RwLock<ClientState>,
    pub job_id: RwLock<Option<u64>>,
    pub work_req: AtomicU64,
    pub unfinished_business: RwLock<Vec<WorkItem>>,
}

impl Client {
    pub fn new(server: Weak<Server>, stream: TcpStream) -> DynResult<Arc<Client>> {
        let addr = stream.peer_addr()?;
        println!("[{}] connected", addr);

        Ok(Arc::new(Client {
            server,
            stream,
            addr,
            mutex: Mutex::new(()),
            state: RwLock::new(ClientState::Unknown),
            job_id: RwLock::new(None),
            work_req: AtomicU64::new(0),
            unfinished_business: RwLock::new(Vec::new()),
        }))
    }

    pub fn cancel_job(&self) -> DynResult<()> {
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

    pub fn update_job_id(&self) -> DynResult<u64> {
        self.cancel_job()?;
        let server = self.server.upgrade().unwrap();
        let job_id = server.job_seq.fetch_add(1, Ordering::SeqCst);
        let mut job_id_opt = self.job_id.write().unwrap();
        *job_id_opt = Some(job_id);
        Ok(job_id)
    }

    pub fn stop_job(&self) -> DynResult<()> {
        let server = self.server.upgrade().unwrap();

        {
            let job_id_opt = self.job_id.read().unwrap();

            if let Some(job_id) = *job_id_opt {
                let mut server_jobs = server.jobs.write().unwrap();
                let mut unfinished_business = self.unfinished_business.write().unwrap();
                let job = server_jobs.iter_mut().find(|j| j.id == job_id);

                if let Some(job) = job {
                    for wi in unfinished_business.iter() {
                        job.work.insert(0, wi.work);
                    }
                }

                unfinished_business.clear();
            }
        }

        server.update_work()
    }

    pub fn send_job(&self, job: &Job) -> DynResult<()> {
        let mut job_id_opt = self.job_id.write().unwrap();
        let mut packet = Vec::new();

        if *job_id_opt != Some(job.id) {
            println!("[{}] send job id={} width={} height={} pos={:?} dir={:?} fov={} scene={} key={} render_mode={} batch_size={}", self.addr, job.id, job.width, job.height, job.cam_pos, job.cam_rot, job.cam_fov, job.scene, job.key, job.render_mode, job.batch_size);
            *job_id_opt = Some(job.id);
            ser::write_u64(&mut packet, job.id);
            ser::write_u64(&mut packet, job.width);
            ser::write_u64(&mut packet, job.height);
            ser::write_vec(&mut packet, job.cam_pos);
            ser::write_vec(&mut packet, job.cam_rot);
            ser::write_f32(&mut packet, job.cam_fov);
            ser::write_f32(&mut packet, job.cam_focus);
            ser::write_f32(&mut packet, job.cam_blur);
            ser::write_str(&mut packet, &job.scene);
            ser::write_str(&mut packet, &job.key);
            ser::write_u64(&mut packet, job.render_mode);
            ser::write_u64(&mut packet, job.batch_size);
            self.write_packet(3, &packet)?;
        }

        Ok(())
    }

    pub fn send_work(&self, job: &mut Job) -> DynResult<()> {
        let job_id_opt = self.job_id.read().unwrap();
        let mut unfinished_business = self.unfinished_business.write().unwrap();

        if *job_id_opt == Some(job.id) {
            let n = self.work_req.load(Ordering::SeqCst) as usize - unfinished_business.len();
            let n = n.min(job.work.len());

            for work in job.work[0..n].iter() {
                if VERBOSE {
                    println!(
                        "[{}] send work begin={} end={}",
                        self.addr, work.begin, work.end
                    );
                }

                let mut packet = Vec::new();

                ser::write_u64(&mut packet, work.begin);
                ser::write_u64(&mut packet, work.end);
                self.write_packet(4, &packet)?;

                unfinished_business.push(WorkItem {
                    work: *work,
                    finished_business: Vec::new(),
                    totally_finished_business: 0,
                });
            }

            job.work.drain(0..n);
        }

        Ok(())
    }

    pub fn write_packet(&self, packet_id: u8, data: &[u8]) -> DynResult<()> {
        let mut packet = Vec::new();

        if VERBOSE {
            println!(
                "[{}] send packet id={} len={}",
                self.addr,
                packet_id,
                data.len()
            );
        }

        ser::write_u64(&mut packet, data.len() as u64);
        ser::write_u8(&mut packet, packet_id);
        ser::write_buf(&mut packet, data);

        let _lock = self.mutex.lock();
        match (&self.stream).write_all(&packet) {
            Ok(_) => Ok(()),
            Err(err) => {
                eprintln!("[{}] {}", self.addr, err);
                Ok(())
            }
        }
    }

    pub fn handle_handshake_packet(&self, mut data: &[u8]) -> DynResult<()> {
        let new_state = ser::read_u8(&mut data)?;
        let protover = ser::read_u64(&mut data)?;
        let server = self.server.upgrade().unwrap();

        if protover != PROTOVER {
            return Err(format!(
                "protocol version mismatch server={} client={}",
                PROTOVER, protover
            )
            .into());
        }

        {
            let new_state = match new_state {
                0 => {
                    let work_req = ser::read_u64(&mut data)?;
                    self.work_req.store(work_req, Ordering::SeqCst);
                    self.write_packet(0, &[])?;
                    println!("[{}] handshake worker", self.addr);
                    ClientState::Worker
                }
                1 => {
                    self.write_packet(0, &[])?;
                    println!("[{}] handshake viewer", self.addr);
                    ClientState::Viewer
                }
                x => {
                    println!("[{}] handshake unknown({})", self.addr, x);
                    ClientState::Unknown
                }
            };

            let mut state = self.state.write().unwrap();
            *state = new_state;
        }

        server.update_jobs()
    }

    pub fn handle_ping_packet(&self, data: &[u8]) -> DynResult<()> {
        println!("[{}] ping", self.addr);
        self.write_packet(2, data)
    }

    pub fn handle_job_request_packet(&self, mut data: &[u8]) -> DynResult<()> {
        let width = ser::read_u64(&mut data)?;
        let height = ser::read_u64(&mut data)?;
        let cam_pos = ser::read_vec(&mut data)?;
        let cam_rot = ser::read_vec(&mut data)?;
        let cam_fov = ser::read_f32(&mut data)?;
        let cam_focus = ser::read_f32(&mut data)?;
        let cam_blur = ser::read_f32(&mut data)?;
        let scene = ser::read_str(&mut data)?;
        let key = ser::read_str(&mut data)?;
        let render_mode = ser::read_u64(&mut data)?;
        let batch_size = ser::read_u64(&mut data)?;
        let server = self.server.upgrade().unwrap();

        {
            let job_id = self.update_job_id()?;
            let mut server_jobs = server.jobs.write().unwrap();
            println!("[{}] recv job id={} width={} height={} pos={:?} dir={:?} fov={} scene={} key={} render_mode={} batch_size={}", self.addr, job_id, width, height, cam_pos, cam_rot, cam_fov, scene, key, render_mode, batch_size);

            server_jobs.push(Job {
                id: job_id,
                width,
                height,
                cam_pos,
                cam_rot,
                cam_fov,
                cam_focus,
                cam_blur,
                scene,
                key,
                render_mode,
                batch_size,
                work: Vec::new(),
            });

            let mut packet = Vec::new();
            ser::write_u64(&mut packet, job_id);
            self.write_packet(3, &packet)?;
        }

        server.update_jobs()
    }

    pub fn handle_send_work_packet(&self, mut data: &[u8]) -> DynResult<()> {
        let begin = ser::read_u64(&mut data)?;
        let end = ser::read_u64(&mut data)?;
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

    pub fn handle_send_results_packet(&self, mut data: &[u8]) -> DynResult<()> {
        let job_id = ser::read_u64(&mut data)?;
        let size = ser::read_u64(&mut data)? as usize;
        let begin = ser::read_u64(&mut data)?;
        let end = ser::read_u64(&mut data)?;
        let results = &data[..size];
        let server = self.server.upgrade().unwrap();

        if VERBOSE {
            println!(
                "[{}] results id={} begin={} end={} size={}",
                self.addr, job_id, begin, end, size
            );
        }

        {
            let mut unfinished_business = self.unfinished_business.write().unwrap();

            if let Some(begin) = {
                let work_item = unfinished_business
                    .iter_mut()
                    .find(|wi| wi.work.begin <= begin && end <= wi.work.end)
                    .ok_or("invalid work unit begin and end")?;

                work_item.totally_finished_business += end - begin;
                work_item.finished_business.push(ResultItem {
                    begin,
                    end,
                    data: results.to_vec(),
                });

                if work_item.totally_finished_business >= work_item.work.end - work_item.work.begin
                {
                    let clients = server.clients.read().unwrap();

                    let client = clients.iter().find(|c| {
                        let client_job_id_opt = c.job_id.read().unwrap();
                        let client_state = c.state.read().unwrap();

                        match *client_state {
                            ClientState::Viewer => *client_job_id_opt == Some(job_id),
                            _ => false,
                        }
                    });

                    if let Some(client) = client {
                        for ri in &work_item.finished_business {
                            let mut packet = Vec::new();

                            ser::write_u64(&mut packet, job_id);
                            ser::write_u64(&mut packet, ri.data.len() as u64);
                            ser::write_u64(&mut packet, ri.begin);
                            ser::write_u64(&mut packet, ri.end);
                            ser::write_buf(&mut packet, &ri.data);

                            client.write_packet(5, &packet)?;
                        }
                    }

                    Some(work_item.work.begin)
                } else {
                    None
                }
            } {
                unfinished_business.retain(|wi| wi.work.begin != begin);
            }
        }

        server.update_work()
    }

    pub fn handle_log_packet(&self, mut data: &[u8]) -> DynResult<()> {
        let string = ser::read_str(&mut data)?;
        println!("[{}] log {:?}", self.addr, string);

        Ok(())
    }

    pub fn handle_packet(&self, packet_id: u8, data: &[u8]) -> DynResult<()> {
        if VERBOSE {
            println!(
                "[{}] recv packet id={} len={}",
                self.addr,
                packet_id,
                data.len()
            );
        }

        match packet_id {
            0x00 => self.handle_handshake_packet(data),
            0x01 => self.handle_ping_packet(data),
            0x03 => self.handle_job_request_packet(data),
            0x04 => self.handle_send_work_packet(data),
            0x05 => self.handle_send_results_packet(data),
            0xFF => self.handle_log_packet(data),
            _ => Ok(()),
        }
    }

    pub fn run_int(&self) -> DynResult<()> {
        let mut packet_buffer = Vec::new();
        let mut read_buffer = [0; 4096];

        loop {
            let count = (&self.stream).read(&mut read_buffer)?;
            packet_buffer.extend(&read_buffer[..count]);

            if count <= 0 {
                return Ok(());
            }

            while packet_buffer.len() >= 9 {
                let mut packet_reader = &packet_buffer[..];
                let length = ser::read_u64(&mut packet_reader)? as usize;
                let packet_id = ser::read_u8(&mut packet_reader)?;

                if packet_buffer.len() < 9 + length {
                    break;
                }

                self.handle_packet(packet_id, ser::read_buf(&mut packet_reader, length)?)?;
                packet_buffer.drain(0..length + 9);
            }
        }
    }

    pub fn run(self: Arc<Self>) {
        let result = self.run_int();
        let server = self.server.upgrade().unwrap();
        let state = self.state.read().unwrap();

        {
            let mut clients = server.clients.write().unwrap();
            clients.retain(|c| Arc::as_ptr(c) != Arc::as_ptr(&self));
        }

        let result = match *state {
            ClientState::Viewer => result.and(self.cancel_job()),
            ClientState::Worker => result.and(self.stop_job()),
            _ => result,
        };

        if let Err(err) = result {
            eprintln!("[{}] {}", self.addr, err);
        }

        println!("[{}] disconnected", self.addr);
    }
}
