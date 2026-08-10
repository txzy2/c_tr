#include "c_math.h"
#include "files.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define FILENAME "data.txt"

int main() {
  printf("Hello World\n");

  int8_t a = 5;
  add_to_int8(&a);

  printf("%d\n", a);

  bool isWrited = write_to_file(FILENAME, "HELLO FROM C");
  if (!isWrited) {
    return -1;
  }

  read_from_file(FILENAME);
  return 0;
}
