all: ppmrw.c
	gcc -DPPMRW_MAIN ppmrw.c -o ppmrw -std=c99

obj: ppmrw.c
	gcc -c ppmrw.c
	ar -cvq libppmrw.a ppmrw.o
	rm ppmrw.o
	mkdir lib
	mv libppmrw.a lib

clean:
	rm -rf ppmrw *.o *.swp *.a
