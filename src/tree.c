#include "../lib/tree.h"
#include "../lib/tar_structure.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int node_init(struct node *n) {
	n = malloc(sizeof(struct node));
	//node->header = NULL;
	n->header = malloc(sizeof(struct tar_header));
	n->parent = NULL;
	n->children = NULL;
	n->children_size = 0;
	n->file = NULL;
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

struct node* get_node_from_path(struct node* n, const char* path) {
	// +1 to remove . from filename

	struct node* aux;
	aux = n;

	// Root
	if (strcmp(aux->header->name + 1, path) == 0) {
		logger(DEBUG, "(tree/get_node_from_path) Node found: %s\n", aux->header->name);
		return aux;
	}

	int modified = 0;
	while (
	(aux->header->name[strlen(aux->header->name) - 1] == '/') ?
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1) - 1) == 0) :
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1)) == 0)
	) {
		logger(DEBUG, "(tree/get_node_from_path) Current node: %s\n", aux->header->name + 1);
		logger(DEBUG, "(tree/get_node_from_path) Children: %d\n", aux->children_size);

		int i;
		for(i = 0; i < aux->children_size; i++) {
			logger(DEBUG, "(tree/get_node_from_path) \tTrying node: %s\n", aux->children[i]->header->name + 1);
			if (
			(aux->children[i]->header->name[strlen(aux->children[i]->header->name) - 1] == '/') ?
			(strncmp(aux->children[i]->header->name + 1, path, strlen(aux->children[i]->header->name + 1) - 1) == 0) :
			((strncmp(aux->children[i]->header->name + 1, path, strlen(aux->children[i]->header->name + 1)) == 0) && (strncmp(aux->children[i]->header->name + 1, path, strlen(path)) == 0))
			) {
				aux = aux->children[i];
				modified = 1;
				break;
			}
		}
		if(!modified) break;
		else modified = 0;
	}

	/*
	if ((strcmp(aux->header->name, n->header->name) == 0) && !modified) {
		logger(DEBUG, "(tree/get_node_from_path) Node not found\n");
		return NULL;
	}
	*/

	/*
	if (
	(aux->header->name[strlen(aux->header->name) - 1] == '/') ?
	(strncmp(aux->header->name + 1, path, strlen(path)) == 0) :
	(strncmp(aux->header->name + 1, path, strlen(path)) == 0)
	) {
	*/
	if (strncmp(aux->header->name + 1, path, strlen(path)) == 0) {
		logger(DEBUG, "(tree/get_node_from_path) Node found: %s\n", aux->header->name);
		return aux;
	}

	logger(DEBUG, "(tree/get_node_from_path) Node not found\n");
	return NULL;
}
