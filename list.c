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

bool add_to_head_list(List *list, const int *val)
{
	struct Node *item = malloc(sizeof(struct Node));
	if (item == NULL)
	{
		return false;
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
	add_to_head_list(&l, &val2);
	add_to_head_list(&l, &val3);

	print_list_debug(&l);

	free_list(&l);
	return 0;
}
