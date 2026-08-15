#include "types.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "user.h"

struct Node
{
	User u;
	struct Node *next;
};

typedef struct
{
	struct Node *head; // откуда pop'аем (dequeue)
	struct Node *tail; // куда push'аем (enqueue)
	i32 size;
} Queue;

bool enqueue(Queue *q, struct Node *node) { return true; }

bool dequeue(Queue *q, struct Node *node) { return true; }

void free_q(Queue *queue)
{
	if (queue == NULL)
	{
		return;
	}

	struct Node *cur = queue->head;
	while (cur != NULL)
	{
		struct Node *next_node = cur->next;
		free(cur);
		cur = next_node;
	}

	queue->head = queue->tail = NULL;
	queue->size = 0;
	return;
}

int main()
{
	i8 result = EXIT_SUCCESS;

	Queue q = {.head = NULL, .tail = NULL, .size = 0};
	printf("HELLO WORLD\n");

	goto cleanup;

cleanup:
	free_q(&q);
	return result;
}
