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

static void print_node(struct Node *node, const char *prefix, bool is_left)
{
	if (node == NULL)
	{
		return;
	}

	printf("%s%s%d\n", prefix, is_left ? "├── " : "└── ", node->value);

	char new_prefix[256];
	snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_left ? "│   " : "    ");

	if (node->left != NULL || node->right != NULL)
	{
		if (node->left != NULL)
		{
			print_node(node->left, new_prefix, node->right != NULL);
		}
		if (node->right != NULL)
		{
			print_node(node->right, new_prefix, false);
		}
	}
}

void print_tree(Tree *tree)
{
	if (tree == NULL || tree->root == NULL)
	{
		printf("(empty)\n");
		return;
	}
	printf("TREE SIZE: %d -> Root: [%p] %d\n", tree->size, tree->root, tree->root->value);
	printf("%d\n", tree->root->value);

	char prefix[256] = "";
	if (tree->root->left != NULL)
	{
		print_node(tree->root->left, prefix, tree->root->right != NULL);
	}
	if (tree->root->right != NULL)
	{
		print_node(tree->root->right, prefix, false);
	}
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
	print_tree(&tree);

	if (!search(&tree, 55))
	{
		printf("NOT FOUND\n");
		result = EXIT_FAILURE;
		goto cleanup;
	}

	printf("FOUND\n");

cleanup:
	free_tree(&tree);
	return result;
}
