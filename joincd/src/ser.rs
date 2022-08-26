pub fn read_buf<'a>(data: &mut &'a [u8], size: usize) -> &'a [u8] {
    let (head, tail) = data.split_at(size);
    *data = tail;
    head
}

pub fn read_u8(data: &mut &[u8]) -> u8 {
    u8::from_le_bytes(read_buf(data, 1).try_into().unwrap())
}

pub fn read_u16(data: &mut &[u8]) -> u16 {
    u16::from_le_bytes(read_buf(data, 2).try_into().unwrap())
}

pub fn read_u32(data: &mut &[u8]) -> u32 {
    u32::from_le_bytes(read_buf(data, 4).try_into().unwrap())
}

pub fn read_u64(data: &mut &[u8]) -> u64 {
    u64::from_le_bytes(read_buf(data, 8).try_into().unwrap())
}

pub fn read_i8(data: &mut &[u8]) -> i8 {
    i8::from_le_bytes(read_buf(data, 1).try_into().unwrap())
}

pub fn read_i16(data: &mut &[u8]) -> i16 {
    i16::from_le_bytes(read_buf(data, 2).try_into().unwrap())
}

pub fn read_i32(data: &mut &[u8]) -> i32 {
    i32::from_le_bytes(read_buf(data, 4).try_into().unwrap())
}

pub fn read_i64(data: &mut &[u8]) -> i64 {
    i64::from_le_bytes(read_buf(data, 8).try_into().unwrap())
}

pub fn read_f32(data: &mut &[u8]) -> f32 {
    f32::from_le_bytes(read_buf(data, 4).try_into().unwrap())
}

pub fn read_f64(data: &mut &[u8]) -> f64 {
    f64::from_le_bytes(read_buf(data, 8).try_into().unwrap())
}

pub fn read_vec(data: &mut &[u8]) -> [f32; 3] {
    [read_f32(data), read_f32(data), read_f32(data)]
}

pub fn read_str(data: &mut &[u8]) -> String {
    let length = read_u64(data) as usize;
    let bytes = read_buf(data, length);
    std::str::from_utf8(bytes).unwrap().to_string()
}

pub fn write_buf(data: &mut Vec<u8>, value: &[u8]) {
    data.extend(value);
}

pub fn write_u8(data: &mut Vec<u8>, value: u8) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_u16(data: &mut Vec<u8>, value: u16) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_u32(data: &mut Vec<u8>, value: u32) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_u64(data: &mut Vec<u8>, value: u64) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_i8(data: &mut Vec<u8>, value: i8) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_i16(data: &mut Vec<u8>, value: i16) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_i32(data: &mut Vec<u8>, value: i32) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_i64(data: &mut Vec<u8>, value: i64) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_f32(data: &mut Vec<u8>, value: f32) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_f64(data: &mut Vec<u8>, value: f64) {
    write_buf(data, &value.to_le_bytes());
}

pub fn write_vec(data: &mut Vec<u8>, value: [f32; 3]) {
    write_f32(data, value[0]);
    write_f32(data, value[1]);
    write_f32(data, value[2]);
}

pub fn write_str(data: &mut Vec<u8>, value: &str) {
    let bytes = value.as_bytes();
    write_u64(data, bytes.len() as u64);
    write_buf(data, bytes);
}
