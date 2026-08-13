#include "books.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>

#define ARGS_LENGTH 4
#define BOOK_NAME 1
#define BOOK_AUTHOR 2
#define BOOK_YEAR 3

void print_menu() { printf("\n==== MENU ====\n1. Add Book\n2. Del Book\n3. Find Book\n0. Exit\n==============\n"); }

void read_input(int *input)
{
	if (scanf("%d", input) != 1)
	{
		printf("INVALID CHOICE\n");
		abort();
	}
}

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
	if (v->b != NULL)
	{
		for (size_t i = 0; i < v->size; i++)
		{
			free(v->b[i]);
		}

		free(v->b);
	}
	v->b = NULL;
	v->size = 0;
	v->capacity = 0;
}

bool delete_book(Vector *v, int id)
{
	if (v == NULL || v->b == NULL || v->size == 0)
	{
		abort();
	}

	for (size_t i = 0; i < v->size; i++)
	{
		if (v->b[i]->id == id)
		{
			free(v->b[i]);

			for (size_t j = i; j < v->size - 1; ++j)
			{
				v->b[j] = v->b[j + 1];
			}

			v->size--;
			v->b[v->size] = NULL;

			return true;
		}
	}
	// TODO: Сделать удаление из файла

	return false;
}

void create_book_data(Book *b, const char *argv[])
{
	if (b == NULL)
	{
		printf("ERROR CREATE BOOK");
		abort();
	}

	b->id = atoi(argv[0]);
	strcpy(b->title, argv[1]);
	strcpy(b->author, argv[2]);

	// =====
	// Возьми строку из argv[BOOK_YEAR], интерпретируй её как десятичное число, результат положи в year, а указатель на
	// место остановки преобразования положи в end.
	char *end;
	unsigned long year = strtoul(argv[3], &end, 10);
	if (*end != '\0')
	{
		fprintf(stderr, "Invalid year\n");
		abort();
	}
	b->year = (uint16_t)year;
	// =====
	b->available = argv[4];
}

bool validate_input(const int *input, Vector *v)
{
	switch (*input)
	{
	case 1:
	{
		char title[100];
		char author[50];
		char year[5];
		printf("Paste <book-title> <author> <year>: ");
		if (scanf("%99s %49s %4s", title, author, year) != 3)
		{
			fprintf(stderr, "INVALID INPUT\n");
			abort();
		}

		Book *b = malloc(sizeof(Book));

		int id_int = v->size + 1;
		int length = snprintf(NULL, 0, "%d", id_int);
		char *id = malloc(length + 1);
		snprintf(id, length + 1, "%d", id_int);

		const char *arr[] = {id, title, author, year, "1"};
		create_book_data(b, arr);

		if (!push_back(v, b))
		{
			free(id);
			free_vec(v);
			abort();
		}

		move_into_storage(b);

		free(id);
		break;
	}
	case 2:
	{
		int id;
		printf("Paste ID: ");

		if (scanf("%d", &id) != 1)
		{
			fprintf(stderr, "INVALID ID\n");
			abort();
		}

		if (!delete_book(v, id))
		{
			fprintf(stderr, "ERROR TO DELETE BOOK\n");
			abort();
		}
		break;
	}
	case 0:
		printf("BYE!\n");
		break;
	default:
		printf("UNDEFINED CHOICE\n");
		return EXIT_FAILURE;
		break;
	}

	return EXIT_SUCCESS;
}

void print_data_to_console(const Vector *v)
{
	for (size_t i = 0; i < v->size; ++i)
	{
		printf("ID: %d | TITLE: %s AUTHOR: %s YEAR: %d\n", v->b[i]->id, v->b[i]->title, v->b[i]->author, v->b[i]->year);
	}
}

bool move_into_storage(Book *b)
{
	char buffer[100];
	int res = snprintf(buffer, sizeof(buffer), "%d;%s;%s;%d;%d", b->id, b->title, b->author, b->year, b->available);

	if (res < 0 || res >= sizeof(buffer))
	{
		return false;
	}

	write_to_file(BOOK_STORAGE_FILENAME, buffer);
	return true;
}

bool load_storage(Vector *v)
{
	FILE *fptr = fopen(BOOK_STORAGE_FILENAME, "r");
	if (fptr == NULL)
	{
		return false;
	}

	char buffStr[256];
	while (fgets(buffStr, sizeof(buffStr), fptr))
	{
		char id[20], title[100], author[50], year[10], available[10];

		if (sscanf(buffStr, "%19[^;];%99[^;];%49[^;];%9[^;];%9[^;\n]", id, title, author, year, available) == 5)
		{
			Book *b = malloc(sizeof(Book));
			if (b == NULL)
			{
				fprintf(stderr, "Memory allocation failed\n");
				continue;
			}

			const char *arr[] = {id, title, author, year, available};
			create_book_data(b, arr);
			push_back(v, b);
		}
	}
	fclose(fptr);
	return true;
}

int main()
{
	Vector v;
	int input;

	vector_init(&v, 1);
	load_storage(&v);
	print_data_to_console(&v);

	print_menu();
	printf("\nINPUT: ");
	read_input(&input);

	if (validate_input(&input, &v))
	{
		free_vec(&v);
		abort();
	}

	print_data_to_console(&v);

	free_vec(&v);
	return EXIT_SUCCESS;
}
