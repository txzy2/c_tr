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

bool enqueue(Queue *q, const User *user)
{
	if (q == NULL || user == NULL)
	{
		return false;
	}

	struct Node *new_node = malloc(sizeof(struct Node));
	if (new_node == NULL)
	{
		return false;
	}
	new_node->u = *user;
	new_node->next = NULL;

	if (q->head == NULL)
	{
		q->head = q->tail = new_node;
	}
	else
	{
		q->tail->next = new_node;
		q->tail = new_node;
	}

	q->size++;
	return true;
}

bool dequeue(Queue *q)
{
	if (q == NULL || q->head == NULL)
	{
		return false;
	}

	struct Node *cur = q->head;
	q->head = cur->next;

	if (q->head == NULL)
	{
		q->tail = NULL;
	}
	free(cur);

	q->size--;
	return true;
}

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

void print_q(Queue *q)
{
	printf("Queue size: %d\nQueue HEAD [%p]\nQueue TAIL [%p]\n", q->size, q->head, q->tail);
	struct Node *cur = q->head;
	while (cur != NULL)
	{
		printf("   [%p] User id: %d, name: %s, email: %s\n", cur, cur->u.id, cur->u.name, cur->u.email);
		cur = cur->next;
	}
}

bool generate_users(Queue *q)
{
	for (i32 i = 0; i < 10; i++)
	{
		char name[256], email[256];
		sprintf(name, "User %d", i + 1);
		sprintf(email, "user%d@example.com", i + 1);

		User u = {.id = i + 1};
		snprintf(u.name, sizeof(u.name), "%s", name);
		u.name[sizeof(u.name) - 1] = '\0';
		snprintf(u.email, sizeof(u.email), "%s", email);
		u.email[sizeof(u.email) - 1] = '\0';

		if (!enqueue(q, &u))
		{
			return false;
		}
	}

	return true;
}

int main()
{
	i8 result = EXIT_SUCCESS;

	Queue q = {.head = NULL, .tail = NULL, .size = 0};

	if (!generate_users(&q))
	{
		result = EXIT_FAILURE;
		goto cleanup;
	}

	print_q(&q);

	for (i32 i = 0; i < 5; i++)
	{

		if (!dequeue(&q))
		{
			result = EXIT_FAILURE;
			goto cleanup;
		}
	}

	print_q(&q);

cleanup:
	free_q(&q);
	return result;
}
