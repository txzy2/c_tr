#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Node
{
	i32 data;
	struct Node *next, *prev;
};

typedef struct
{
	i32 size;
	struct Node *head, *end;
} List;

bool add_to_end_list(List *list, const i32 *val)
{
	struct Node *item = malloc(sizeof(struct Node));
	if (item == NULL)
	{
		return false;
	}

	item->data = *val;
	item->next = item->prev = NULL;

	if (list->end == NULL)
	{
		list->head = list->end = item;
	}
	else
	{
		item->prev = list->end;
		list->end->next = item;
		list->end = item;
	}

	list->size++;
	return true;
}

struct Node *add_to_head_list(List *list, const i32 *val)
{
	struct Node *item = malloc(sizeof(struct Node));
	if (item == NULL)
	{
		return NULL;
	}

	item->data = *val;
	item->next = item->prev = NULL;

	if (list->head == NULL)
	{
		list->head = list->end = item;
	}
	else
	{
		item->next = list->head;
		list->head->prev = item;
		list->head = item;
	}
	list->size++;

	return item;
}

bool delete_node(List *list, struct Node *node)
{
	if (list == NULL || list->head == NULL || node == NULL)
	{
		return false;
	}

	if (node == list->head)
	{
		list->head = node->next;
		if (list->head != NULL)
		{
			list->head->prev = NULL;
		}
	}
	else
	{
		node->prev->next = node->next;
		if (node->next != NULL)
		{
			node->next->prev = node->prev;
		}
	}

	if (node == list->end)
	{
		list->end = node->prev;
	}
	list->size--;

	free(node);
	return true;
}

bool delete_first_node(List *list)
{
	if (list == NULL || list->head == NULL)
	{
		return false;
	}

	delete_node(list, list->head);

	return true;
}

bool delete_last_node(List *list)
{
	// TODO: Find a first node in cyrcle like (If node haven't got prev)
	if (list == NULL || list->end == NULL)
	{
		return false;
	}

	delete_node(list, list->end);
	return true;
}

void print_list_debug(const List *list)
{
	printf("head=%p end=%p size=%d\n", (void *)list->head, (void *)list->end, list->size);
	struct Node *cur = list->head;
	while (cur != NULL)
	{
		printf("  [%p] data=%d prev=%p next=%p\n", (void *)cur, cur->data, (void *)cur->prev, (void *)cur->next);
		cur = cur->next;
	}
}

void free_list(List *list)
{
	struct Node *cur = list->head;

	while (cur != NULL)
	{
		struct Node *next_node = cur->next;
		free(cur);
		cur = next_node;
	}

	list->head = list->end = NULL;
	list->size = 0;
}

int main()
{
	List l = {.head = NULL, .end = NULL, .size = 0};
	const i32 val = 5, val2 = 10, val3 = 12;

	add_to_end_list(&l, &val);
	struct Node *val_item2 = add_to_head_list(&l, &val2);
	add_to_head_list(&l, &val3);

	print_list_debug(&l);

	if (!delete_node(&l, val_item2))
	{
		fprintf(stderr, "ERROR REMOVE");
		return 1;
	}
	delete_first_node(&l);

	print_list_debug(&l);
	free_list(&l);
	return 0;
}
