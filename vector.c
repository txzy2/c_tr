#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool vector_init(Vector *v, size_t element_size)
{
	if (v == NULL || element_size == 0)
	{
		return false;
	}

	v->size = 0;
	v->capacity = INITIAL_CAPACITY;
	v->element_size = element_size;

	v->data = malloc(v->capacity * v->element_size);

	if (v->data == NULL)
	{
		v->size = v->capacity = v->element_size = 0;
		return false;
	}

	return true;
}

bool vector_push(Vector *v, const void *element)
{
	if (v == NULL || element == NULL)
	{
		return false;
	}

	if (v->size == v->capacity)
	{
		size_t new_capacity = v->capacity * 2;
		void *el = realloc(v->data, new_capacity * v->element_size);

		if (el == NULL)
		{
			return false;
		}

		v->data = el;
		v->capacity = new_capacity;
	}

	memcpy((char *)v->data + v->size * v->element_size, element, v->element_size);
	v->size++;

	return true;
}

void *vector_get(Vector *v, size_t index)
{
	if (v == NULL || index >= v->size)
	{
		return NULL;
	}

	return (char *)v->data + index * v->element_size;
}

void vector_free(Vector *v)
{
	if (v == NULL)
	{
		return;
	}

	free(v->data);
	v->data = NULL;
	v->size = v->capacity = v->element_size = 0;
}

int main()
{
	Vector v;
	int i = 3;
	vector_init(&v, sizeof(int));
	vector_push(&v, &i);

	for (size_t i = 0; i < v.size; ++i)
	{
		int *val = vector_get(&v, i);
		printf("%d\n", *val);
	}

	vector_free(&v);

	Vector v2;
	User u = {.id = 1, .name = "Anton"};
	vector_init(&v2, sizeof(User));
	vector_push(&v2, &u);

	for (size_t i = 0; i < v2.size; ++i)
	{
		User *val = vector_get(&v2, i);
		printf("id: %d, name: %s\n", val->id, val->name);
	}

	vector_free(&v2);

	return 0;
}
