.PHONY: b-main b-structs 

b-main:
	gcc -Wall -Wextra -std=c17 -o main main.c files.c

b-structs:
	gcc -Wall -Wextra -std=c17 -o structs structs.c files.c
