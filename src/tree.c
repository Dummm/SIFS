#include "../lib/tree.h"
#include "../lib/tar_structure.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct node* get_node_from_path(struct node* n, const char* path) {
	// +1 to remove . from filename
	logger(DEBUG, "(tree/get_node_from_path) Started with path: %s\n", path);

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

		int i;
		for(i = 0; i < aux->children_size; i++) {
			logger(DEBUG, "(tree/get_node_from_path) \tTrying node: %s\n", aux->children[i]->header->name);

			if ((
				(aux->children[i]->header->name[strlen(aux->children[i]->header->name) - 1] == '/')
			) ? (
				(strncmp(aux->children[i]->header->name + 1, path, strlen(aux->children[i]->header->name + 1) - 1) == 0) &&
				(
					((path[strlen(aux->children[i]->header->name + 1) - 1] == '\0') && ((aux->children[i]->header->name + 1)[strlen(aux->children[i]->header->name + 1) - 1] == '/')) ||
					((path[strlen(aux->children[i]->header->name + 1) - 1] == '/')  && ((aux->children[i]->header->name + 1)[strlen(aux->children[i]->header->name + 1) - 1] == '/'))
				)
			) : (
				(strncmp(aux->children[i]->header->name + 1, path, strlen(aux->children[i]->header->name + 1)) == 0) &&
				(strncmp(aux->children[i]->header->name + 1, path, strlen(path)) == 0) &&
				(strlen(aux->children[i]->header->name + 1) == strlen(path))
			)) {
				aux = aux->children[i];
				modified = 1;
				break;
			}

		}
		if(!modified) break;
		else modified = 0;
	}

	if (strncmp(aux->header->name + 1, path, strlen(path)) == 0) {
		logger(DEBUG, "(tree/get_node_from_path) Node found: %s\n", aux->header->name);
		return aux;
	}

	logger(DEBUG, "(tree/get_node_from_path) Node not found\n");
	return NULL;
}
