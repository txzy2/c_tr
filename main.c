#include "files.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("Hello World\n");

	if (!write_to_file(FILENAME, "HELLO FROM C", "w"))
	{
		return -1;
	}

	int nums[] = {1, 2, 3, 4};
	int length = sizeof(nums) / sizeof(nums[0]);

	printf("[");
	for (int i = 0; i < length; i++)
	{
		printf("%d", nums[i]);
		if (i < length - 1)
			printf(", ");
	}
	printf("]\n");

	char *str_nums = malloc(100);
	int pos = 0;

	for (int i = 0; i < length; ++i)
	{
		pos += snprintf(str_nums + pos, 100 - pos, "%d%s", nums[i], (i < length - 1) ? ", " : "");
	}

	if (!write_to_file(FILENAME, str_nums, "a"))
	{
		free(str_nums);
		return -1;
	}

	free(str_nums);
	printf("\n");
	read_from_file(FILENAME);

	return 0;
}
