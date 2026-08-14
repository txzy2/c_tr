#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>

#define INITIAL_CAPACITY 2

typedef struct
{
	int id;
	char name[50];
} User;

typedef struct
{
	void *data;
	size_t size;
	size_t capacity;
	size_t element_size;
} Vector;

bool vector_init(Vector *v, size_t element_size);
bool vector_push(Vector *v, const void *element);
void *vector_get(Vector *v, size_t index);
void vector_free(Vector *v);

#endif
