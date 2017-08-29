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

/*
 * I create this enumeration to give context to the "3" or "6" passed in to the
 * main function. Can refer to them by an human-understandable name this way.
 */
enum FileType {
    PP3_PPM = 3,
    PP6_PPM = 6
};

/*
 * Entry point for the program
 * Mainly does usage checking and setup for file input/output
 */
int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [3 or 6] [input] [output]\n", argv[0]);
        exit(1);
    }

    int format = atoi(argv[1]);
    char *in_fname = argv[2];
    char *out_fname = argv[3];

    FILE *input = fopen(in_fname, "r");

    if (!input) {
        fprintf(stderr, "Error: unable to open input file (%s)!\n", in_fname);
        exit(1);
    }

    return 0;
}
