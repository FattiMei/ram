build:
	gcc -o test main.c ram.c stream.c array.c

run: build
	./test
