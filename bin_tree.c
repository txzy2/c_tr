#include "types.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
    50
   /  \
  30    70
 / \    / \
20 40  60  80
*/

struct Node
{
	i32 value;
	struct Node *left;
	struct Node *right;
};

typedef struct
{
	struct Node *root;
	i32 size;
} Tree;

bool insert(Tree *tree, i32 value)
{
	if (tree == NULL)
	{
		return false;
	}

	struct Node **cur = &tree->root;
	while (*cur != NULL)
	{
		if (value == (*cur)->value)
		{
			return false;
		}
		cur = value > (*cur)->value ? &(*cur)->right : &(*cur)->left;
	}

	struct Node *node = malloc(sizeof(struct Node));
	if (node == NULL)
	{
		return false;
	}
	node->value = value;
	node->left = node->right = NULL;
	*cur = node;
	tree->size++;

	return true;
}

bool remove_node(Tree *tree, i32 value);

bool search(Tree *tree, i32 value)
{
	if (tree == NULL || tree->root == NULL || value <= 0)
	{
		return false;
	}

	struct Node *cur = tree->root;
	while (cur != NULL)
	{
		if (cur->value == value)
		{
			return true;
		}
		cur = cur->value > value ? cur->left : cur->right;
	}

	return false;
}

static void free_node(struct Node *node)
{
	if (node == NULL)
	{
		return;
	}

	free_node(node->left);
	free_node(node->right);
	free(node);
}

void free_tree(Tree *tree)
{
	if (tree == NULL)
	{
		return;
	}

	free_node(tree->root);
	tree->root = NULL;
}

void generate_tree(Tree *tree)
{
	for (i32 i = 0; i < 99; i++)
	{
		i32 value = rand() % 100 + 1;
		insert(tree, value);
	}
}

int main()
{
	srand((unsigned int)time(NULL));
	i8 result = EXIT_SUCCESS;

	Tree tree = {.root = NULL};
	generate_tree(&tree);
	printf("TREE SIZE: %d -> Root: [%p] %d\n", tree.size, tree.root, tree.root->value);

	i32 val = 55;
	if (!search(&tree, val))
	{
		printf("%d IS NOT FOUND\n", val);
		result = EXIT_FAILURE;
		goto cleanup;
	}

	printf("FOUND\n");

cleanup:
	free_tree(&tree);
	return result;
}
