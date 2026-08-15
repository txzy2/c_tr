#include "types.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct
{
	i32 id;
	char name[256];
	char email[256];
} User;

struct Node
{
	User u; // Тут не храню указатель потому что структура данных маленькая
	struct Node *next;
};

typedef struct
{
	struct Node *top;
	i32 size;
} Stack;

// Return first el
User *peek(Stack *stack);

// Add el to top of stack
bool push(Stack *stack, const User *user);

// Delete el top of the stack
bool pop(Stack *stack);

// Is stack empty?
bool is_empty(Stack *stack);

int main()
{
	i8 result = 0;

	printf("HELLO WORLD\n");
	goto cleanup;

cleanup:
	return result;
}
