all: ppmrw.c
	gcc ppmrw.c -o ppmrw -std=c99

clean:
	rm -rf ppmrw *.o *.swp
