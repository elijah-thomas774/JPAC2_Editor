#include "buffer.hpp"


Buffer::Buffer(std::string& path) : data()
{
    std::ifstream file_buff(path, 
        std::ios::binary | std::ios::in | std::ios::ate);
    if(file_buff.is_open())
    {
        i32 size = file_buff.tellg();
        data = std::vector<u8>(size);
        file_buff.seekg(0, std::ios::beg);
        file_buff.read((char *)&data[0], size);
        file_buff.close(); 

    } else {
        std::cout << "Could not locate file" << std::endl;
    }
}

Buffer Buffer::read_slice_amount(i32 offset, i32 n)
{
    return read_slice(offset, offset+n);
}
Buffer Buffer::read_slice(i32 offset, i32 end)
{
    std::vector<u8> temp;
    assert(offset < data.size());
    assert(end <= data.size());
    for(offset; offset<end; offset++)
    {
        temp.push_back(data.at(offset));
    }
    return Buffer(temp);
}

void Buffer::append(Buffer &newBuff){
    append(data);
}
void Buffer::append(std::vector<u8> &dat){
    data.reserve(data.size() + dat.size());
    for (u8 temp : dat)
    {
        data.push_back(temp);
    }
}

f32   Buffer::read_f32  (i32 offset){
    u32 temp = read_u32(offset);
    return *(f32*)&temp;
}
i8    Buffer::read_i8   (i32 offset){
    assert(offset < data.size());
    return data.at(offset);
}
i16   Buffer::read_i16  (i32 offset){
    assert(data.size() > offset+1);
    u16 t1 = data.at(offset)   & 0xFF;
    u16 t2 = data.at(offset+1) & 0xFF;
    return ((t1 << 8) | t2);
}
i32   Buffer::read_i32  (i32 offset){
    return (read_u32(offset)&0xFFFFFFFF);
}
u8    Buffer::read_u8   (i32 offset){
    assert(offset < data.size());
    return data.at(offset);
}
u16   Buffer::read_u16  (i32 offset){
    assert(data.size() > offset+1);
    u16 t1 = data.at(offset)   & 0xFF;
    u16 t2 = data.at(offset+1) & 0xFF;
    return ((t1 << 8) | t2);
}
u32   Buffer::read_u32  (i32 offset){
    assert(data.size() > offset+3);
    u32 t1 = data.at(offset)   & 0xFF;
    u32 t2 = data.at(offset+1) & 0xFF;
    u32 t3 = data.at(offset+2) & 0xFF;
    u32 t4 = data.at(offset+3) & 0xFF;
    u32 temp = (t1 << 24) | (t2 << 16) | (t3 << 8) | (t4);
    return temp;
}
vec3f Buffer::read_vec3f(i32 offset){
    vec3f type;
    type.push_back(read_f32(offset));
    type.push_back(read_f32(offset+4));
    type.push_back(read_f32(offset+8));
    return type;
}
vec2f Buffer::read_vec2f(i32 offset){
    vec2f type;
    type.push_back(read_f32(offset));
    type.push_back(read_f32(offset+4));
    return type;
}
vec3s Buffer::read_vec3s(i32 offset){
    vec3s type;
    type.push_back(read_u16(offset));
    type.push_back(read_u16(offset+2));
    type.push_back(read_u16(offset+4));
    return type;
}
std::string Buffer::read_string(i32 offset, i32 n){
    std::string temp;
    for(i32 i = 0; i < n; i++)
    {
        assert(i+offset < data.size());
        if(data.at(i+offset) == '\0')
            break;
        temp.append(1, (char)data.at(i+offset));
    }
    return temp;
}

std::vector<f32> Buffer::read_vec_f32(i32 offset, i32 n){
    std::vector<f32> temp; 
    for(i32 i = 0; i < n*4; i+=4)
    {
        temp.push_back(read_f32(offset+i));
    }
    return temp;
}
std::vector<i8 > Buffer::read_vec_i8 (i32 offset, i32 n){
    std::vector<i8> temp; 
    for(i32 i = 0; i < n; i+=1)
    {
        temp.push_back(read_i8(offset+i));
    }
    return temp;
}
std::vector<i16> Buffer::read_vec_i16(i32 offset, i32 n){
    std::vector<i16> temp; 
    for(i32 i = 0; i < n*2; i+=2)
    {
        temp.push_back(read_i16(offset+i));
    }
    return temp;
}
std::vector<i32> Buffer::read_vec_i32(i32 offset, i32 n){
    std::vector<i32> temp; 
    for(i32 i = 0; i < n*4; i+=4)
    {
        temp.push_back(read_i32(offset+i));
    }
    return temp;
}
std::vector<u8 > Buffer::read_vec_u8 (i32 offset, i32 n){
    std::vector<u8> temp; 
    for(i32 i = 0; i < n; i+=1)
    {
        temp.push_back(read_u8(offset+i));
    }
    return temp;
}
std::vector<u16> Buffer::read_vec_u16(i32 offset, i32 n){
    std::vector<u16> temp; 
    for(i32 i = 0; i < n*2; i+=2)
    {
        temp.push_back(read_u16(offset+i));
    }
    return temp;
}
std::vector<u32> Buffer::read_vec_u32(i32 offset, i32 n){
    std::vector<u32> temp; 
    for(i32 i = 0; i < n*4; i+=4)
    {
        temp.push_back(read_u32(offset+i));
    }
    return temp;
}

void Buffer::write(f32 value){
    u32 temp = *(u32*)&value;
    write(temp);
}
void Buffer::write(i8 value){
    data.push_back((value&0xFF));
}
void Buffer::write(i16 value){
   write((u16)(value&0xFFFF));
}
void Buffer::write(i32 value){
    write((u32)(value&0xFFFFFFFF));
}
void Buffer::write(u8 value){
    data.push_back((value&0xFF));
}
void Buffer::write(u16 value){
    write((u8)((value>>8)&0xFF));
    write((u8)((value>>0)&0xFF));
}
void Buffer::write(u32 value){
    write((u8)((value>>24)&0xFF));
    write((u8)((value>>16)&0xFF));
    write((u8)((value>> 8)&0xFF));
    write((u8)((value>> 0)&0xFF));
}

void Buffer::write(std::vector<u8>  &value){
    for(u8 temp : value)
        write(temp);
}
void Buffer::write(std::vector<u16> &value){
    for(u16 temp : value)
        write(temp);
}
void Buffer::write(std::vector<u32> &value){
    for(u32 temp : value)
        write(temp);
}
void Buffer::write(std::vector<i8>  &value){
    for(i8 temp : value)
        write(temp);
}
void Buffer::write(std::vector<i16> &value){
    for(i16 temp : value)
        write(temp);
}
void Buffer::write(std::vector<i32> &value){
    for(i32 temp : value)
        write(temp);
}
void Buffer::write(std::vector<f32> &value){
    for(f32 temp : value)
        write(temp);
}
void Buffer::check_size(i32 offset, i32 n = 1)
{
    if (data.size() < offset+n)
    {
        assert(!"Reading data will result in out of bounds access");
    }
}