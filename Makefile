build:
	mkdir -p lib/
	gcc -o lib/getopt_dos.o -c src/getopt_dos.c -I include/

build-test: build
	mkdir -p bin/
	gcc -o bin/getopt_dos-test tests/getopt_dos.c lib/getopt_dos.o -I include/

test: build-test
	bin/getopt_dos-test