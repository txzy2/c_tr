.PHONY: build run

build:
	gcc -Wall -Wextra -std=c17 -o main main.c files.c

run: build
	./main
