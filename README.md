# Introduction
PPM Read/write (ppmrw) is a program capable of reading and writing PPM files in
the P3/P6 format and converting between the two formats.

# Installation
Simply use the Makefile to build/compile the program.

    make

# Usage
In order to use ppmrw, provide the binary with the following arguments:

    ./ppmrw [3|6] [input] [output]

Example usage:

    ./ppmrw 3 cutecat_binary.ppm cutecat_ascii.ppm

This will convert the cutecat_binary.ppm file to a new PPM file
"cutecat_ascii.ppm" in the P3 format.

# Author
Trey Tangeman <tct49@nau.edu>
