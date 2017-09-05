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
    P1_PPM = 1,
    P2_PPM = 2,
    P3_PPM = 3,
    P4_PPM = 4,
    P5_PPM = 5,
    P6_PPM = 6,
    P7_PPM = 7
};

struct pixel {
    u8 r, g, b;
};

struct ppm_pixmap {
    enum ppm_format format;
    u32 width, height, color_range;
    struct pixel *pixmap;
};

