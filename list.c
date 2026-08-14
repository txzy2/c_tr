#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Node
{
	int data;
	struct Node *next;
	struct Node *prev;
};

typedef struct
{
	struct Node *head;
	struct Node *end;
	int size;
} List;

bool add_to_end_list(List *list, const int *val)
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

struct Node *add_to_head_list(List *list, const int *val)
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

	node->prev = node->next = 0;
	node->data = 0;
	list->size--;

	free(node);
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
	const int val = 5, val2 = 10, val3 = 12;

	add_to_end_list(&l, &val);
	struct Node *val_item2 = add_to_head_list(&l, &val2);
	add_to_head_list(&l, &val3);

	print_list_debug(&l);

	if (!delete_node(&l, val_item2))
	{
		fprintf(stderr, "ERROR REMOVE");
		return 1;
	}

	print_list_debug(&l);
	free_list(&l);
	return 0;
}
