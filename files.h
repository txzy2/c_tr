#ifndef FILES_H
#define FILES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define READ_LENGTH 100
#define FILENAME "data.txt"
#define NAMES_FILE "names.txt"

bool write_to_file(const char *filename, const char *text);
size_t readFromFileWithArray(const char *filename, char arr[][30]);
void read_from_file(const char *filename);

#endif
