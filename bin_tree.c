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
} Tree;

bool insert(Tree *tree, i32 value)
{
	if (tree == NULL)
	{
		return false;
	}

	struct Node *node = malloc(sizeof(struct Node));
	if (node == NULL)
	{
		return false;
	}
	node->value = value;
	node->left = node->right = NULL;

	struct Node **cur = &tree->root;
	while (*cur != NULL)
	{
		if (value == (*cur)->value)
		{
			return false;
		}
		cur = value > (*cur)->value ? &(*cur)->right : &(*cur)->left;
	}
	*cur = node;

	return true;
}

bool remove_node(Tree *tree, i32 value);

bool search(Tree *tree, i32 value);

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

int main()
{

	srand((unsigned int)time(NULL));

	i8 result = EXIT_SUCCESS;
	printf("Hello, World!\n");

	Tree tree = {.root = NULL};
	for (i32 i = 0; i < 99; i++)
	{
		i32 value = rand() % 100 + 1; // от 1 до 100 включительно
		insert(&tree, value);
	}

	print_tree(&tree);

cleanup:
	free_tree(&tree);
	return result;
}
