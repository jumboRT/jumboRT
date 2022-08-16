pub fn read_u8(data: &[u8]) -> u8 {
    u8::from_le_bytes(data.try_into().unwrap())
}

pub fn read_u16(data: &[u8]) -> u16 {
    u16::from_le_bytes(data.try_into().unwrap())
}

pub fn read_u32(data: &[u8]) -> u32 {
    u32::from_le_bytes(data.try_into().unwrap())
}

pub fn read_u64(data: &[u8]) -> u64 {
    u64::from_le_bytes(data.try_into().unwrap())
}

pub fn read_i8(data: &[u8]) -> i8 {
    i8::from_le_bytes(data.try_into().unwrap())
}

pub fn read_i16(data: &[u8]) -> i16 {
    i16::from_le_bytes(data.try_into().unwrap())
}

pub fn read_i32(data: &[u8]) -> i32 {
    i32::from_le_bytes(data.try_into().unwrap())
}

pub fn read_i64(data: &[u8]) -> i64 {
    i64::from_le_bytes(data.try_into().unwrap())
}

pub fn read_f32(data: &[u8]) -> f32 {
    f32::from_le_bytes(data.try_into().unwrap())
}

pub fn read_f64(data: &[u8]) -> f64 {
    f64::from_le_bytes(data.try_into().unwrap())
}

pub fn write_u8(data: &mut Vec<u8>, value: u8) {
    data.extend(value.to_le_bytes());
}

pub fn write_u16(data: &mut Vec<u8>, value: u16) {
    data.extend(value.to_le_bytes());
}

pub fn write_u32(data: &mut Vec<u8>, value: u32) {
    data.extend(value.to_le_bytes());
}

pub fn write_u64(data: &mut Vec<u8>, value: u64) {
    data.extend(value.to_le_bytes());
}

pub fn write_i8(data: &mut Vec<u8>, value: i8) {
    data.extend(value.to_le_bytes());
}

pub fn write_i16(data: &mut Vec<u8>, value: i16) {
    data.extend(value.to_le_bytes());
}

pub fn write_i32(data: &mut Vec<u8>, value: i32) {
    data.extend(value.to_le_bytes());
}

pub fn write_i64(data: &mut Vec<u8>, value: i64) {
    data.extend(value.to_le_bytes());
}

pub fn write_f32(data: &mut Vec<u8>, value: f32) {
    data.extend(value.to_le_bytes());
}

pub fn write_f64(data: &mut Vec<u8>, value: f64) {
    data.extend(value.to_le_bytes());
}
