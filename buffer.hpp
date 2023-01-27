#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "common_types.hpp"
#include <assert.h>
#include <fstream>
#include <iostream>

class Buffer {
    public:
        Buffer(std::string path);
        Buffer() : data() {};
         
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

        template<typename T>
        void write(T value);

        template<typename T>
        void write(std::vector<T> value);

        std::string read_string(i32 offset, i32 n);

        template<typename T>
        void read_data(T &dataType, i32 offset);
        
        std::vector<u8> data;
};

#endif


