#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include <vector>
#include <cstdint>
#include <string>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int32_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint32_t u64;
typedef float f32;

typedef std::vector<f32> vec2f;
typedef std::vector<f32> vec3f;
typedef std::vector<u16> vec3s;

struct ColorValues {
    u8 r,g,b,a;
};

union Color
{
    ColorValues values;
    u32 color_string;
};

#endif