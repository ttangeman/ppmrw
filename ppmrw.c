/*
 * CS430: Project 1
 * Author: Trey Tangeman
 * Description: ppmrw is able to read, write, and convert between files in the
 * PPM P3/P6 format.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "ppmrw.h"

/*
 * Reads a file into memory and returns a struct with a pointer
 * to the memory and the file size.
 * NOTE: user is responsible for freeing the memory
 */
struct file_contents get_file_contents(FILE *fh)
{
    void *memory;
    size_t size;

    fseek(fh, 0, SEEK_END);
    size = ftell(fh);
    rewind(fh);

    memory = malloc(size);
    fread(memory, 1, size, fh);

    struct file_contents result = {memory, size};
    return result;
}

/*
 * Wrapper function for advancing until a whitespace.
 */
static void read_until_whitespace(char *ppm_memory, u32 *offset)
{
    while (!isspace(ppm_memory[(*offset)++]));
}

/*
 * Wrapper function for advancing through whitespace.
 */
static void read_whitespace(char *ppm_memory, u32 *offset)
{
    while (isspace(ppm_memory[*offset])) {
        *(offset)++;
    }
}

/*
 * This function is a wrapper around the operation of advancing through all
 * comments until we hit the next line.
 */
static void read_comments(char *ppm_memory, u32 *offset)
{
    while (ppm_memory[*offset] == '#') {
        while (ppm_memory[(*offset)++] != '\n');
    }
}

/*
 * This function is a wrapper around getting an ASCII value and converting
 * it to binary. The ASCII value should be delimited by whitespace!
 *
 * IMPORTANT: this function also handles the proceeding whitespaces and comments,
 * for ease of use.
 */
static int get_binary_value(char *ppm_memory, u32 *offset_ptr)
{
    // Get the start of the string and the end of the string
    u32 start = *offset_ptr;
    read_until_whitespace(ppm_memory, offset_ptr);
    u32 end = *offset_ptr;

    // Allocate a string for the characters and copy the values from
    // memory. We also need to null terminate for atoi to work.
    char ascii[end - start];
    strncpy(ascii, ppm_memory + start, end - start);
    ascii[end - start] = '\0';

    // Get us to the next value
    read_whitespace(ppm_memory, offset_ptr);
    read_comments(ppm_memory, offset_ptr);

    return atoi(ascii);
}

/*
 * Initializes a PPM pixmap struct with the values in the header:
 * format - Either P3 or P6
 * width, height - the width and height of the pixmap
 * max_color_depth - the maximum value for the RGB colors (e.g. 255)
 *
 * Returns a status code identifying the error.
 */
int init_ppm_pixmap(struct ppm_pixmap *pm, struct file_contents fc)
{
    char *ascii_mem = (char *)fc.memory;
    char magic[2] = {ascii_mem[0], ascii_mem[1]};

    if (strncmp(magic, "P3", sizeof("P3")) == 0) {
        pm->format = P3_PPM;
    } else if (strncmp(magic, "P6", sizeof("P6")) == 0) {
        pm->format = P6_PPM;
    } else {
       return INVALID_FORMAT;
    }

    u32 offset = 0;

    read_until_whitespace(ascii_mem, &offset);
    read_whitespace(ascii_mem, &offset);
    read_comments(ascii_mem, &offset);

    // Parse out the width, height, and bits per channel from the header
    // NOTE: these functions account for whitespace and comments!
    s32 width = get_binary_value(ascii_mem, &offset);
    s32 height = get_binary_value(ascii_mem, &offset);
    s32 bits_per_channel = get_binary_value(ascii_mem, &offset);

    // Error checking for negative widths and heights
    // and bits_per_channel != 255
    if (width < 0) {
        return INVALID_WIDTH;
    } else if (height < 0) {
        return INVALID_HEIGHT;
    } else if (bits_per_channel != MAX_BITS_PER_CHANNEL) {
        return INVALID_BITS_PER_CHANNEL;
    }

    pm->width = width;
    pm->height = height;
    pm->bits_per_channel = bits_per_channel;
    pm->pixmap = malloc(sizeof(struct pixel) * (width * height));

    struct pixel *pixels = pm->pixmap;

    if (pm->format == P3_PPM) {
        // Get the binary values of the ASCII pixels
        for (int i = 0; i < (width * height); i++) {
            pixels[i].r = get_binary_value(ascii_mem, &offset);
            pixels[i].g = get_binary_value(ascii_mem, &offset);
            pixels[i].b = get_binary_value(ascii_mem, &offset);
        }
    } else {
        // Cast the binary blob into a pixel array
        memcpy(pm->pixmap, fc.memory + offset, sizeof(struct pixel) * width * height);
    }

    return INIT_SUCCESS;
}

/*
 * This function handles the error messages for the error_code
 * from the init_ppm_pixmap function.
 */
static void handle_init_error_code(int error_code)
{
    switch (error_code) {
        case INVALID_FORMAT:
            fprintf(stderr, "Error: input file has an invalid format specified in header.\n");
        break;
        case INVALID_WIDTH:
            fprintf(stderr, "Error: input file has an invalid width specified in header.\n");
        break;
        case INVALID_HEIGHT:
            fprintf(stderr, "Error: input file has an invalid height specified in header.\n");
        break;
        case INVALID_BITS_PER_CHANNEL:
            fprintf(stderr, "Error: input file has an invalid bits per channel specified in header.\n");
        break;
    }
}

/*
 * ---- Entry point for the program ----
 * Usage and error checking is done in the main function, before
 * it begins processing the file.
 */
int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [3|6] [input] [output]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    s32 format = atoi(argv[1]);
    char *in_fname = argv[2];
    char *out_fname = argv[3];

    if (format != P3_PPM && format != P6_PPM) {
        fprintf(stderr, "Error: invalid format \"%d\" specified!\n", format);
        fprintf(stderr, "Valid formats: 3|6\n");
        exit(EXIT_FAILURE);
    }

    FILE *input = fopen(in_fname, "r");

    if (!input) {
        fprintf(stderr, "Error: unable to open input file %s!\n", in_fname);
        exit(EXIT_FAILURE);
    }

    struct file_contents fc = get_file_contents(input);
    fclose(input);

    struct ppm_pixmap pm = {0};
    s32 status_code = init_ppm_pixmap(&pm, fc);
    free(fc.memory);

    if (status_code != INIT_SUCCESS) {
        handle_init_error_code(status_code);
        exit(EXIT_FAILURE);
    } else if (pm.format == format) {
        printf("Nothing to be changed. File is already in P%d format.\n", format);

        FILE *output = fopen(out_fname, "w");
        fwrite(fc.memory, 1, fc.size, output);
        fclose(output);
    } else {
        printf("Changed file from P%d to P%d.\n", pm.format, format);
        FILE *output = fopen(out_fname, "w");

        // The format we want to convert to
        char *fmt = (pm.format == P6_PPM ? "P3" : "P6");

        char width[32];
        char height[32];
        char bits_per_channel[32];
        // This is the most portable way to do itoa
        sprintf(width, "%d", pm.width);
        sprintf(height, "%d", pm.height);
        sprintf(bits_per_channel, "%d", pm.bits_per_channel);

        fputs(fmt, output);
        fputc('\n', output);
        fputs(width, output);
        fputc(' ', output);
        fputs(height, output);
        fputc('\n', output);
        fputs(bits_per_channel, output);
        fputc('\n', output);

        // Only for going to P3 do we need to write out ASCII
        // This is the "conversion" part of the code.
        if (pm.format == P6_PPM) {
            u32 num_pixels = pm.width * pm.height;
            char r[32];
            char g[32];
            char b[32];

            for (int i = 0; i < num_pixels; i++) {
                sprintf(r, "%d", pm.pixmap[i].r);
                sprintf(g, "%d", pm.pixmap[i].g);
                sprintf(b, "%d", pm.pixmap[i].b);

                fputs(r, output);
                fputc(' ', output);
                fputs(g, output);
                fputc(' ', output);
                fputs(b, output);
                fputc(' ', output);
            }
        } else {
            fwrite(pm.pixmap, 1, sizeof(struct pixel) * pm.width * pm.height, output);
        }

        fclose(output);
    }

    free(pm.pixmap);

    return 0;
}
