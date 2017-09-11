#pragma once

/*
 * Typedefs/Enums
 * ==============
 */
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;

#define MAX_BITS_PER_CHANNEL    255

enum ppm_format {
    P1_PPM = 1,
    P2_PPM = 2,
    P3_PPM = 3,
    P4_PPM = 4,
    P5_PPM = 5,
    P6_PPM = 6,
    P7_PPM = 7
};

// These values are used for the init_ppm_pixmap function
enum status_code {
    INIT_SUCCESS,
    INVALID_FORMAT,
    INVALID_WIDTH,
    INVALID_HEIGHT,
    INVALID_BITS_PER_CHANNEL
};

/*
 * Struct definitions
 * ==================
 */
struct pixel {
    u8 r, g, b;
};

struct ppm_pixmap {
    enum ppm_format format;     // P3 or P6 for pixmaps
    u32 width, height;
    u32 bits_per_channel;       // 0 to bits_per_channel for RGB
    struct pixel *pixmap;       // NOTE: allocated during init, so needs to be freed
};

struct file_contents {
    void *memory;
    size_t size;
};

/*
 * Function declarations
 * ====================
 */
struct file_contents get_file_contents(FILE *fh);
int init_ppm_pixmap(struct ppm_pixmap *pm, struct file_contents fc);

