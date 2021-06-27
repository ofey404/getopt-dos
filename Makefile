build:
	mkdir -p lib/
	gcc -o lib/getopt-dos.o -c src/getopt-dos.c -I include/

build-test: build
	mkdir -p bin/
	gcc -o bin/getopt-dos-test tests/getopt-dos.c lib/getopt-dos.o -I include/

test: build-test
	bin/getopt-dos-test