#include "arrays.h"

void print_array(const int *arr, int length) {
	printf("[");
	for (int i = 0; i < length; i++) {
		printf("%d", arr[i]);
		if (i < length - 1)
			printf(", ");
	}
	printf("]\n");
}
