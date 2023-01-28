#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "common_types.hpp"
#include <assert.h>
#include <fstream>
#include <iostream>

class Buffer {
    public:
        Buffer() : data() {};
        Buffer(std::string& path);
        Buffer(std::vector<u8> dat) : data(dat) {}
        Buffer read_slice_amount(i32 offset, i32 n);
        Buffer read_slice(i32 offset, i32 end);
        void check_size(i32 offset, i32 n);
        f32   read_f32  (i32 offset);
        i8    read_i8   (i32 offset);
        i16   read_i16  (i32 offset);
        i32   read_i32  (i32 offset);
        u8    read_u8   (i32 offset);
        u16   read_u16  (i32 offset);
        u32   read_u32  (i32 offset);
        vec3f read_vec3f(i32 offset);
        vec2f read_vec2f(i32 offset);
        vec3s read_vec3s(i32 offset);

        std::vector<f32> read_vec_f32(i32 offset, i32 n);
        std::vector<i8 > read_vec_i8 (i32 offset, i32 n);
        std::vector<i16> read_vec_i16(i32 offset, i32 n);
        std::vector<i32> read_vec_i32(i32 offset, i32 n);
        std::vector<u8 > read_vec_u8 (i32 offset, i32 n);
        std::vector<u16> read_vec_u16(i32 offset, i32 n);
        std::vector<u32> read_vec_u32(i32 offset, i32 n);

        std::string read_string(i32 offset, i32 n);

        void write(f32 value);
        void write(i8 value);
        void write(i16 value);
        void write(i32 value);
        void write(u8 value);
        void write(u16 value);
        void write(u32 value);

        void write(std::vector<u8>  &value);
        void write(std::vector<u16> &value);
        void write(std::vector<u32> &value);
        void write(std::vector<i8>  &value);
        void write(std::vector<i16> &value);
        void write(std::vector<i32> &value);
        void write(std::vector<f32> &value);
        
        void append(Buffer &newBuff);
        void append(std::vector<u8> &dat);
        std::vector<u8> data;
};

#endif


