#include "files.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

void read_from_file(const char *filename) {
	FILE *rf = fopen(filename, "r");

	if (rf == NULL) {
		return;
	}

	char file_str[READ_LENGTH];
	while (fgets(file_str, READ_LENGTH, rf)) {
		printf("READ FROM FILE: %s", file_str);
	}

	fclose(rf);
}

bool write_to_file(const char *filename, const char *text) {
	FILE *f = fopen(filename, "a");
	if (f == NULL) {
		return false;
	}

	fprintf(f, "%s\n", text);
	fclose(f);

	return true;
}
