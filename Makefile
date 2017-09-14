all: ppmrw.c
	gcc -DPPMRW_MAIN ppmrw.c -o ppmrw -std=c99

obj: ppmrw.c
	gcc -c ppmrw.c

clean:
	rm -rf ppmrw *.o *.swp
