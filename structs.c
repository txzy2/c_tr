#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
	char name[30];
	int8_t age;
};

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

bool updateUser(struct User *u, const char *name, const int age) {
	if (name == NULL || name[0] == '\0' || strlen(name) >= 30) {
		return false;
	}

	strcpy(u->name, name);
	u->age = age;

	return true;
}
int main() {

	struct User *u = createUser("Anton", 25);
	printf("BASE NAME: %s\n", u->name);
	updateUser(u, "Dima", 21);
	printf("AFTER RENAME: %s\n", u->name);

	free(u);
	return 0;
}
