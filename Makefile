all: build run

build:
	gcc -lncurses -o termatrix *.c

run: build
	./termatrix
