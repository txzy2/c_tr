#include "types.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
bool push(Stack *stack, const User *user)
{
	if (stack == NULL || user == NULL)
	{
		return false;
	}

	struct Node *new_node = malloc(sizeof(struct Node));
	if (new_node == NULL)
	{
		return false;
	}

	new_node->u = *user; // Разыменовыли
	new_node->next = stack->top;
	stack->top = new_node;
	stack->size++;

	return true;
}

// Delete el top of the stack
bool pop(Stack *stack);

// Is stack empty?
bool is_empty(Stack *stack);

// Delete stack
void free_s(Stack *stack)
{
	struct Node *cur = stack->top;
	while (cur != NULL)
	{
		struct Node *next_node = cur->next;
		free(cur);
		cur = next_node;
	}
	stack->top = NULL;
	stack->size = 0;
}

void print_debug(const Stack *stack)
{
	printf("top=%p size=%d\n", (void *)stack->top, stack->size);
	struct Node *cur = stack->top;
	while (cur != NULL)
	{
		printf("  [%p] id=%d name=%s email=%s next=%p\n", (void *)cur, cur->u.id, cur->u.name, cur->u.email,
		       (void *)cur->next);
		cur = cur->next;
	}
}

int main()
{
	i8 result = EXIT_SUCCESS;

	User u = {.id = 1, .name = "Anton", .email = "test@test.ru"};
	Stack s = {.top = NULL, .size = 0};

	if (!push(&s, &u))
	{
		fprintf(stderr, "ERROR TO PUSH\n");
		result = EXIT_FAILURE;
		goto cleanup;
	}

	print_debug(&s);

cleanup:
	free_s(&s);
	return result;
}
