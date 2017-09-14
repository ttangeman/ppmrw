all: ppmrw.c
	gcc -DPPMRW_MAIN ppmrw.c -o ppmrw -std=c99

clean:
	rm -rf ppmrw *.o *.swp
