.PHONY: b-main b-structs 

b-main:
	gcc -Wall -Wextra -std=c17 -o bin/main main.c files.c

b-structs:
	gcc -Wall -Wextra -std=c17 -o bin/structs structs.c files.c
