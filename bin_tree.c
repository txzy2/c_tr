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

bool remove_node(Tree *tree, i32 value)
{
	if (tree == NULL)
	{
		return false;
	}

	struct Node *parent = NULL;
	struct Node *cur = tree->root;
	while (cur != NULL && cur->value != value)
	{
		parent = cur;
		cur = cur->value > value ? cur->left : cur->right;
	}

	if (cur == NULL)
	{
		return false;
	}

	if (cur->left == NULL && cur->right == NULL)
	{
		if (parent == NULL)
		{
			tree->root = NULL;
		}
		else if (parent->left == cur)
		{
			parent->left = NULL;
		}
		else
		{
			parent->right = NULL;
		}
		free(cur);
	}
	else if (cur->left == NULL || cur->right == NULL)
	{
		struct Node *child = (cur->left != NULL) ? cur->left : cur->right;
		if (parent == NULL)
		{
			tree->root = child;
		}
		else if (parent->left == cur)
		{
			parent->left = child;
		}
		else
		{
			parent->right = child;
		}
		free(cur);
	}
	else
	{
		struct Node *successor_parent = cur;
		struct Node *successor = cur->right;

		while (successor->left != NULL)
		{
			successor_parent = successor;
			successor = successor->left;
		}

		cur->value = successor->value;
		if (successor_parent->left == successor)
		{
			successor_parent->left = successor->right;
		}
		else
		{
			successor_parent->right = successor->right;
		}
		free(successor);
	}

	return true;
}

bool search(Tree *tree, i32 value)
{
	if (tree == NULL || tree->root == NULL)
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

	if (!remove_node(&tree, val))
	{
		printf("%d IS NOT FOUND\n", val);
		result = EXIT_FAILURE;
		goto cleanup;
	}

	printf("FOUND\n");
	printf("TREE SIZE: %d -> Root: [%p] %d\n", tree.size, tree.root, tree.root->value);

cleanup:
	free_tree(&tree);
	return result;
}
