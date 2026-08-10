#ifndef FILES_H
#define FILES_H

#include <stdbool.h>
#include <stdint.h>

#define READ_LENGTH 100

bool write_to_file(const char *filename, const char *text);
void read_from_file(const char *filename);

#endif
