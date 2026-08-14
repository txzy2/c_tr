.PHONY: b-main b-structs b-book b-vector b-list clean

b-main:
	gcc -Wall -Wextra -std=c17 -o bin/main main.c files.c

b-structs:
	gcc -Wall -Wextra -std=c17 -o bin/structs structs.c files.c

b-book:
	gcc -Wall -Wextra -std=c17 -o bin/books books.c files.c

b-vector:
	gcc -Wall -Wextra -std=c17 -o bin/vector vector.c

b-list:
	gcc -Wall -Wextra -std=c17 -o bin/list list.c

clean:
	rm -f bin/*
