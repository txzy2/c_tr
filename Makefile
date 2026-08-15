.PHONY: b-main b-structs b-book b-vector b-list b-stack b-queue v-main v-structs v-book v-vector v-stack v-queue clean

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

b-stack:
	gcc -Wall -Wextra -std=c17 -o bin/stack stack.c

b-queue:
	gcc -Wall -Wextra -std=c17 -o bin/queue queue.c

v-main: b-main
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/main

v-structs: b-structs
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/structs

v-book: b-book
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/books

v-vector: b-vector
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/vector

v-list: b-list
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/list

v-stack: b-stack
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/stack

v-queue:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/queue

clean:
	rm -f bin/*
