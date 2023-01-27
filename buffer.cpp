#include "buffer.hpp"


Buffer::Buffer(std::string path)
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

f32   Buffer::read_f32  (i32 offset){
    f32 type;
    read_data(type, offset);
    return type;
}
i8    Buffer::read_i8   (i32 offset){
    i8 type;
    read_data(type, offset);
    return type;
}
i16   Buffer::read_i16  (i32 offset){
    i16 type;
    read_data(type, offset);
    return type;
}
i32   Buffer::read_i32  (i32 offset){
    i32 type;
    read_data(type, offset);
    return type;
}
u8    Buffer::read_u8   (i32 offset){
    u8 type;
    read_data(type, offset);
    return type;
}
u16   Buffer::read_u16  (i32 offset){
    u16 type;
    read_data(type, offset);
    return type;
}
u32   Buffer::read_u32  (i32 offset){
    u32 type;
    read_data(type, offset);
    return type;
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

template<typename T>
void write(T value);

template<typename T>
void write(std::vector<T> value);

std::string read_string(i32 offset, i32 n);

template<typename T>
void Buffer::read_data(T &dataType, i32 offset)
{
    if ( sizeof(dataType) == 1)
    {
        assert(data.size() > offset);
        dataType = (data.at(offset) & 0xFF);
    }
    else if (sizeof(dataType) == 2)
    {        
        assert(data.size() > offset+1);
        u16 t1 = data.at(offset)   & 0xFF;
        u16 t2 = data.at(offset+1) & 0xFF;
        dataType = ((t1 << 8) | t2);
    }
    else if (sizeof(dataType) == 4)
    {
        assert(data.size() > offset+3);
        u32 t1 = data.at(offset)   & 0xFF;
        u32 t2 = data.at(offset+1) & 0xFF;
        u32 t3 = data.at(offset+2) & 0xFF;
        u32 t4 = data.at(offset+3) & 0xFF;
        u32 temp = (t1 << 24) | (t2 << 16) | (t3 << 8) | (t4);
        dataType = *(T*)&temp; // allows float converison
    }

}
void Buffer::check_size(i32 offset, i32 n = 1)
{
    if (data.size() < offset+n)
    {
        assert(!"Reading data will result in out of bounds access");
    }
}