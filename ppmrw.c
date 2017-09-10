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
    // Read until we reach the next line
    while (!isspace(ascii_mem[offset++]));

    // Advance through all of the comments
    while (ascii_mem[offset] == '#') {
        while(ascii_mem[offset++] != '\n');
    }

    if (isspace(ascii_mem[offset])) {
        while (!isspace(ascii_mem[offset++]));
    }
    printf("%s", ascii_mem + offset);

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
        case INVALID_COLOR_DEPTH:
            fprintf(stderr, "Error: input file has an invalid color depth specified in header.\n");
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

    if (status_code != INIT_SUCCESS) {
        handle_init_error_code(status_code);
        free(fc.memory);
        exit(EXIT_FAILURE);
    } else if (pm.format == format) {
        printf("Nothing to be changed. File is already in P%d format.\n", format);
    } else {
        printf("Changed file from P%d to P%d.\n", pm.format, format);
    }

    free(fc.memory);
    return 0;
}
