#include "c_math.h"

void add_to_int8(int8_t *valptr) {
  if (valptr == NULL || *valptr == INT8_MAX) {
    return;
  }

  *valptr += 1;
}
