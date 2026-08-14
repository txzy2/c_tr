#include "books.h"
#include "files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_data_to_console(const Vector *v)
{
	for (size_t i = 0; i < v->size; ++i)
	{
		printf("ID: %d | TITLE: %s AUTHOR: %s YEAR: %d\n", v->b[i]->id, v->b[i]->title, v->b[i]->author, v->b[i]->year);
	}
}

void print_menu() { printf("\n==== MENU ====\n1. Add Book\n2. Del Book\n0. Exit\n==============\n"); }

bool read_input(int *input)
{
	if (scanf("%d", input) != 1)
	{
		fprintf(stderr, "INVALID CHOICE\n");
		while (getchar() != '\n')
			;
		return false;
	}
	return true;
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
		return false;
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

			if (v->size == 0)
			{
				write_to_file(BOOK_STORAGE_FILENAME, "", "w");
				return true;
			}

			for (size_t i = 0; i < v->size; ++i)
			{
				move_into_storage(v->b[i], i == 0 ? "w" : "a");
			}

			return true;
		}
	}

	return false;
}

Book *create_book(int id, const char *title, const char *author, int year, bool available)
{
	Book *b = malloc(sizeof(Book));
	if (b == NULL)
	{
		return NULL;
	}

	b->id = id;

	strncpy(b->title, title, sizeof(b->title) - 1);
	b->title[sizeof(b->title) - 1] = '\0';

	strncpy(b->author, author, sizeof(b->author) - 1);
	b->author[sizeof(b->author) - 1] = '\0';

	b->year = year;
	b->available = available;

	return b;
}

bool validate_input(const int *input, Vector *v)
{
	switch (*input)
	{
	case 1:
	{
		char title[100], author[50], year_str[5];

		printf("Paste <book-title> <author> <year>: ");
		if (scanf("%99s %49s %4s", title, author, year_str) != 3)
		{
			fprintf(stderr, "INVALID INPUT\n");
			return EXIT_FAILURE;
		}

		if (strchr(title, ';') || strchr(author, ';'))
		{
			fprintf(stderr, "Symbol ';' is not allowed\n");
			return EXIT_FAILURE;
		}

		char *end;
		unsigned long year = strtoul(year_str, &end, 10);
		if (*end != '\0' || year > UINT16_MAX)
		{
			fprintf(stderr, "Invalid year\n");
			return EXIT_FAILURE;
		}

		int id = (v->size > 0) ? v->b[v->size - 1]->id + 1 : 1;
		Book *b = create_book(id, title, author, (int)year, true);
		if (b == NULL)
		{
			fprintf(stderr, "Memory allocation failed\n");
			return EXIT_FAILURE;
		}

		if (!push_back(v, b))
		{
			free(b);
			fprintf(stderr, "Failed to add book\n");
			return EXIT_FAILURE;
		}

		move_into_storage(b, "a");
		break;
	}
	case 2:
	{
		int id;
		printf("Paste ID: ");

		if (scanf("%d", &id) != 1)
		{
			fprintf(stderr, "INVALID ID\n");
			return EXIT_FAILURE;
		}

		if (!delete_book(v, id))
		{
			fprintf(stderr, "ERROR TO DELETE BOOK\n");
			return EXIT_FAILURE;
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

bool move_into_storage(Book *b, const char *mode)
{
	char buffer[100];
	int res = snprintf(buffer, sizeof(buffer), "%d;%s;%s;%d;%d", b->id, b->title, b->author, b->year, b->available);

	if (res < 0 || (size_t)res >= sizeof(buffer))
	{
		return false;
	}

	return write_to_file(BOOK_STORAGE_FILENAME, buffer, mode);
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
		char id_str[20], title[100], author[50], year_str[10], avail_str[10];

		if (sscanf(buffStr, "%19[^;];%99[^;];%49[^;];%9[^;];%9[^;\n]", id_str, title, author, year_str, avail_str) == 5)
		{
			int id = atoi(id_str);
			int year = atoi(year_str);
			bool available = atoi(avail_str);

			Book *b = create_book(id, title, author, year, available);
			if (b == NULL)
			{
				fprintf(stderr, "Memory allocation failed\n");
				continue;
			}

			if (!push_back(v, b))
			{
				free(b);
			}
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

	while (true)
	{
		print_data_to_console(&v);
		print_menu();
		printf("\nINPUT: ");

		if (!read_input(&input))
		{
			continue;
		}

		if (validate_input(&input, &v) && input != 0)
		{
			continue;
		}

		if (input == 0)
		{
			break;
		}
	}

	free_vec(&v);

	return EXIT_SUCCESS;
}
