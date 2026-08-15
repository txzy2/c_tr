#include "types.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//     50
//    /  \
//   30    70
//  / \    / \
// 20 40  60  80

struct Node
{
	i32 value;
	struct Node *left;
	struct Node *right;
};

typedef struct
{
	struct Node *root;
} Tree;

bool insert(Tree *tree, i32 value);

bool remove_node(Tree *tree, i32 value);

bool search(Tree *tree, i32 value);

void print_tree(Tree *tree);

void free_tree(Tree *tree);

int main()
{
	i8 result = EXIT_SUCCESS;
	printf("Hello, World!\n");

	goto cleanup;

cleanup:
	return result;
}
