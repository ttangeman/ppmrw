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

static bool valid_magic_number(struct file_contents fc)
{
    char *ascii_mem = (char *)fc.memory;
    char magic[2] = {ascii_mem[0], ascii_mem[1]};

    if (strncmp(magic, "P3", sizeof("P3")) == 0) ||
       (strncmp(magic, "P6", sizeof("P6")) == 0)
        return true;
    else
        return false;
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

    free(fc.memory);
    return 0;
}
