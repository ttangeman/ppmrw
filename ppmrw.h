#pragma once

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

enum ppm_format {
    PP3_PPM = 3,
    PP6_PPM = 6
};

struct pixel {
    u8 r, g, b, a;
};

struct ppm {
    enum ppm_format format;
    u32 width, height, color_range;
    struct pixel *pixmap;
};

