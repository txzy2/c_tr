#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
	char name[30];
	int8_t age;
};

typedef struct {
	struct User **u;
	size_t size;
	size_t capacity;
} UserVector;

void vector_init(UserVector *uv, size_t capacity) {
	uv->u = malloc(capacity * sizeof(struct User *));
	uv->size = 0;
	uv->capacity = capacity;
}

void push_back(UserVector *uv, struct User *u) {
	if (uv->size == uv->capacity) {
		uv->capacity *= 2;
		uv->u = realloc(uv->u, uv->capacity * sizeof(struct User *));
	}

	uv->u[uv->size] = u;
	uv->size++;
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

struct User *createUser(const char *name, int age) {
	if (name == NULL || name[0] == '\0' || strlen(name) >= 30 || age <= 0 || age > INT8_MAX) {
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

int main() {
	UserVector uv;
	vector_init(&uv, 1);

	struct User *u = createUser("Anton", 25);

	if (u) {
		push_back(&uv, u);

		if (!updateName(uv.u[0], "Dima")) {
			free_vec(&uv);
			printf("ERROR EDIT USER\n");
			return 0;
		}
	}

	for (size_t i = 0; i < uv.size; i++) {
		printf("%p\n", uv.u[i]);
		printf("%s (%d years)\n", uv.u[i]->name, uv.u[i]->age);
	}

	free_vec(&uv);
	return 0;
}
