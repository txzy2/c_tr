#include "files.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct User {
	char name[30];
	uint8_t age;
};

typedef struct {
	struct User **u; // array if pointers
	size_t size;
	size_t capacity;
} UserVector;

bool vector_init(UserVector *uv, size_t capacity) {
	if (uv == NULL || capacity == 0) {
		return false;
	}

	uv->u = malloc(capacity * sizeof(*uv->u));
	if (uv->u == NULL) {
		return false;
	}

	uv->size = 0;
	uv->capacity = capacity;

	return true;
}

bool push_back(UserVector *uv, struct User *u) {
	if (uv == NULL || u == NULL) {
		return false;
	}

	if (uv->size == uv->capacity) {
		size_t new_capacity = uv->capacity * 2;

		// realloc может вернуть NULL.
		// Используем временный указатель, чтобы не потерять старый блок.
		//        realloc()
		//           │
		//           ▼
		//        ┌─────┐
		//        │ tmp │
		//        └─────┘
		//           │
		//  успешно? │
		//    ┌──────┴──────┐
		//   YES            NO
		//    │              │
		//    ▼              ▼
		// uv->u=tmp      старый uv->u
		//               остаётся жив
		struct User **tmp = realloc(uv->u, new_capacity * sizeof(*uv->u));

		if (tmp == NULL) {
			return false;
		}

		uv->u = tmp;
		uv->capacity = new_capacity;
	}

	uv->u[uv->size++] = u;

	return true;
}

void free_vec(UserVector *uv) {
	for (size_t i = 0; i < uv->size; i++) {
		free(uv->u[i]);
	}
	free(uv->u);
	uv->u = NULL;
	uv->size = 0;
	uv->capacity = 0;
}

struct User *createUser(const char *name, uint8_t age) {
	if (name == NULL || name[0] == '\0' || strlen(name) >= 30 || age <= 0 || age > UINT8_MAX - 1) {
		return NULL;
	}

	struct User *u = malloc(sizeof(struct User));

	if (u == NULL) {
		return NULL;
	}

	strcpy(u->name, name);
	u->age = age;

	return u;
}

bool updateName(struct User *u, const char *name) {
	if (name == NULL || name[0] == '\0' || strlen(name) >= 30) {
		return false;
	}

	strcpy(u->name, name);
	return true;
}

bool load_users(UserVector *uv, const char *filename) {
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		return false;
	}

	char buffStr[100];
	while (fgets(buffStr, 100, fptr)) {
		char name[30];
		int age;

		if (sscanf(buffStr, "%29s (%d years)", name, &age) == 2) {
			struct User *u = createUser(name, age);
			push_back(uv, u);
		}
	}

	fclose(fptr);
	return true;
}

int main() {
	srand(time(0));

	UserVector uv;
	vector_init(&uv, 1);

	load_users(&uv, FILENAME);

	char names[100][30];
	size_t names_count = readFromFileWithArray(NAMES_FILE, names);

	if (names_count == 0) {
		free_vec(&uv);
		return EXIT_FAILURE;
	}

	int name_index = rand() % names_count;
	int age = rand() % 100 + 1;

	struct User *u = createUser(names[name_index], age);

	if (u) {
		push_back(&uv, u);

		char buffer[100];
		snprintf(buffer, sizeof(buffer), "%s (%d years)", u->name, u->age);
		write_to_file(FILENAME, buffer);
	}

	read_from_file(FILENAME);
	free_vec(&uv);
	return 0;
}
