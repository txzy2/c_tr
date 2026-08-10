.PHONY: build run

build:
	gcc -Wall -Wextra -std=c17 -o main main.c files.c c_math.c

run: build
	./main
