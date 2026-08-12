#ifndef BOOKS_H
#define BOOKS_H

#include "files.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	int id;
	char title[100];
	char author[50];
	int year;
	bool available;
} Book;

typedef struct
{
	Book **b; // array if pointers
	size_t size;
	size_t capacity;
} Vector;

bool vector_init(Vector *v, size_t capacity);
void free_vec(Vector *v);
bool push_back(Vector *v, Book *b);

void print_menu();
bool validate_input(const int *input, Vector *v);
void read_input(int *input);

bool delete_book(Vector *v, int id);

bool move_into_storage(Book *b);

#endif
