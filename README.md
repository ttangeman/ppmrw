# Introduction
PPM Read/write (ppmrw) is a program capable of reading and writing PPM files in
the P3/P6 format.

# Installation
Simply use the Makefile to build/compile the program.

    make

# Usage
In order to use ppmrw, simply provide the binary with the following arguments:

    ./ppmrw [3/6] [input] [output]

Example usage:

    ./ppmrw 3 cutecat.ppm cutecat.ppm

This will convert the cutecat.ppm file to a new PPM file in the P3 format with
the same filename.

# Author
Trey Tangeman <tct49@nau.edu>
