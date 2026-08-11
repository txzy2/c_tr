#include "books.h"

#define ARGS_LENGTH 4
#define BOOK_NAME 1
#define BOOK_AUTHOR 2
#define BOOK_YEAR 3

bool vector_init(Vector *v, size_t capacity)
{
	if (v == NULL || capacity == 0)
	{
		return false;
	}

	v->b = malloc(capacity * sizeof(*v->b));
	if (v->b == NULL)
	{
		return false;
	}

	v->size = 0;
	v->capacity = capacity;

	return true;
}

bool push_back(Vector *v, Book *b)
{
	if (v == NULL || b == NULL)
	{
		return false;
	}

	if (v->size == v->capacity)
	{
		size_t new_capacity = v->capacity * 2;
		Book **tmp = realloc(v->b, new_capacity * sizeof(*v->b));

		if (tmp == NULL)
		{
			return false;
		}

		v->b = tmp;
		v->capacity = new_capacity;
	}

	v->b[v->size++] = b;
	return true;
}

void free_vec(Vector *v)
{
	for (size_t i = 0; i < v->size; i++)
	{
		free(v->b[i]);
	}
	free(v->b);
	v->b = NULL;
	v->size = 0;
	v->capacity = 0;
}

int main(int argc, const char *argv[])
{
	Vector v;
	vector_init(&v, 1);

	if (argc != ARGS_LENGTH)
	{
		printf("No command-line arguments passed. (<book-name> <author-name> <year>)\n");
		return EXIT_FAILURE;
	}

	Book *b = malloc(sizeof(Book));
	if (b == NULL)
	{
		printf("ERROR CREATE BOOK");
		return EXIT_FAILURE;
	}

	strcpy(b->title, argv[BOOK_NAME]);
	strcpy(b->author, argv[BOOK_AUTHOR]);
	b->available = true;

	// =====
	// Возьми строку из argv[BOOK_YEAR], интерпретируй её как десятичное число, результат положи в year, а указатель на
	// место остановки преобразования положи в end.
	char *end;
	unsigned long year = strtoul(argv[BOOK_YEAR], &end, 10);
	if (*end != '\0')
	{
		printf("Invalid year\n");
		return EXIT_FAILURE;
	}
	b->year = (uint16_t)year;
	// =====

	push_back(&v, b);

	for (size_t i = 0; i < v.size; ++i)
	{
		printf("TITLE: %s AUTHOR: %s YEAR: %d\n", v.b[i]->title, v.b[i]->author, v.b[i]->year);
	}

	free_vec(&v);
	return EXIT_SUCCESS;
}
