pub fn read_u8(vec: &[u8], index: usize) -> u8 {
    let mut buf = [0; 1];
    buf.copy_from_slice(&vec[index..index + 1]);
    u8::from_le_bytes(buf)
}

pub fn read_u16(vec: &[u8], index: usize) -> u16 {
    let mut buf = [0; 2];
    buf.copy_from_slice(&vec[index..index + 2]);
    u16::from_le_bytes(buf)
}

pub fn read_u32(vec: &[u8], index: usize) -> u32 {
    let mut buf = [0; 4];
    buf.copy_from_slice(&vec[index..index + 4]);
    u32::from_le_bytes(buf)
}

pub fn read_u64(vec: &[u8], index: usize) -> u64 {
    let mut buf = [0; 8];
    buf.copy_from_slice(&vec[index..index + 8]);
    u64::from_le_bytes(buf)
}

pub fn write_u8(vec: &mut Vec<u8>, value: u8) {
    vec.extend(value.to_le_bytes());
}

pub fn write_u16(vec: &mut Vec<u8>, value: u16) {
    vec.extend(value.to_le_bytes());
}

pub fn write_u32(vec: &mut Vec<u8>, value: u32) {
    vec.extend(value.to_le_bytes());
}

pub fn write_u64(vec: &mut Vec<u8>, value: u64) {
    vec.extend(value.to_le_bytes());
}

