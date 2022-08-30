use crate::DynResult;

pub fn read_buf<'a>(data: &mut &'a [u8], size: usize) -> DynResult<&'a [u8]> {
    if data.len() < size {
        Err("not enough bytes in buffer".into())
    } else {
        let (head, tail) = data.split_at(size);
        *data = tail;
        Ok(head)
    }
}

pub fn read_u8(data: &mut &[u8]) -> DynResult<u8> {
    Ok(u8::from_le_bytes(read_buf(data, 1)?.try_into()?))
}

pub fn read_u16(data: &mut &[u8]) -> DynResult<u16> {
    Ok(u16::from_le_bytes(read_buf(data, 2)?.try_into()?))
}

pub fn read_u32(data: &mut &[u8]) -> DynResult<u32> {
    Ok(u32::from_le_bytes(read_buf(data, 4)?.try_into()?))
}

pub fn read_u64(data: &mut &[u8]) -> DynResult<u64> {
    Ok(u64::from_le_bytes(read_buf(data, 8)?.try_into()?))
}

pub fn read_i8(data: &mut &[u8]) -> DynResult<i8> {
    Ok(i8::from_le_bytes(read_buf(data, 1)?.try_into()?))
}

pub fn read_i16(data: &mut &[u8]) -> DynResult<i16> {
    Ok(i16::from_le_bytes(read_buf(data, 2)?.try_into()?))
}

pub fn read_i32(data: &mut &[u8]) -> DynResult<i32> {
    Ok(i32::from_le_bytes(read_buf(data, 4)?.try_into()?))
}

pub fn read_i64(data: &mut &[u8]) -> DynResult<i64> {
    Ok(i64::from_le_bytes(read_buf(data, 8)?.try_into()?))
}

pub fn read_f32(data: &mut &[u8]) -> DynResult<f32> {
    Ok(f32::from_le_bytes(read_buf(data, 4)?.try_into()?))
}

pub fn read_f64(data: &mut &[u8]) -> DynResult<f64> {
    Ok(f64::from_le_bytes(read_buf(data, 8)?.try_into()?))
}

pub fn read_vec(data: &mut &[u8]) -> DynResult<[f32; 3]> {
    Ok([read_f32(data)?, read_f32(data)?, read_f32(data)?])
}

pub fn read_str(data: &mut &[u8]) -> DynResult<String> {
    let length = read_u64(data)? as usize;
    let bytes = read_buf(data, length)?;
    Ok(std::str::from_utf8(bytes)?.to_string())
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
