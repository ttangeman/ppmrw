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
 * No need to include these in the header because they are only used
 * for internal (static) functions.
 * ==============================================
 */

struct file_contents {
    void *memory;
    size_t size;
};

// ==============================================

/*
 * Reads a file into memory and returns a struct with a pointer
 * to the memory and the file size.
 */
static struct file_contents get_file_contents(FILE *fh)
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
 * Initializes a PPM header struct with the values in the header:
 * format - Either P3 or P6
 * width, height - the width and height of the pixmap
 * max_color_depth - the maximum value for the RGB colors (e.g. 255)
 */
int init_ppm_pixmap(struct ppm_pixmap *pm, void *file_memory)
{
    char *ascii_mem = (char *)file_memory;
    char magic[2] = {ascii_mem[0], ascii_mem[1]};

    if (strncmp(magic, "P3", sizeof("P3")) == 0)
        pm->format = P3_PPM;
    else if (strncmp(magic, "P6", sizeof("P6")) == 0)
        pm->format = P6_PPM;
    else
       return INIT_FAILED;

    u32 offset = 0;
    // Read until we reach the line with the width, height.
    while (!isspace(ascii_mem[offset++]));

    printf("%s", ascii_mem + offset);

    return INIT_SUCCESS;
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

    if (!init_ppm_pixmap(&pm, fc.memory)) {
        free(fc.memory);
        fprintf(stderr, "Error: unable to parse input file header.\n");
        exit(EXIT_FAILURE);
    } else if (pm.format == format) {
        printf("Nothing to be changed. File is already in P%d format.\n", format);
    } else {
        printf("Changed file from P%d to P%d.\n", pm.format, format);
    }

    free(fc.memory);
    return 0;
}
