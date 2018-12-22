#include "tree.h"
#include "tar_structure.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int node_init(struct node *n) {
	n = malloc(sizeof(struct node));
	printf("a");
	//node->header = NULL;
	n->header = malloc(sizeof(struct tar_header));
	printf("a");
	n->parent = NULL;
	printf("a");
	n->children = NULL;
	printf("a");
	n->children_size = 0;
	printf("a");
	n->file = NULL;
	printf("a");
	return 1;
}
int tree_add_child(struct node *parent, struct node *new_node) {
	parent->children_size++;
	parent->children = malloc(new_node->children_size * sizeof(struct node *));
	parent->children[new_node->children_size - 1] = new_node;

	new_node->parent = parent;

	return 1;
}
int tree_remove_node(struct node *node) {
	struct node *parent = node->parent;

	int i;
	for (i = 0; i < parent->children_size; i++) {
		if (parent->children[i] == node) {
			while (i < parent->children_size - 1) {
				parent->children[i] = parent->children[i + 1];
				i++;
			}
			parent->children_size--;
			return 1;
		}
	}

	return 0;
}
