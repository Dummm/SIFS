#include "../lib/unlink.h"

int sifs_unlink(const char* path) {
	logger(DEBUG, "[unlink] Started on path: %s\n", path);

	struct fuse_context* context;
	context=fuse_get_context();

	struct node* root;
	root=(struct node*)context->private_data;
	struct node *deleted = get_node_from_path(root, path);

	struct node* parent = deleted->parent;

	int del_index;
	for(int i = 0; i < parent->children_size; i++) {
		if(!strcmp(parent->children[i]->header->name, deleted->header->name)){
			del_index = i;
		}
	}

	free(deleted);
	for (int i = del_index + 1; i < parent->children_size; i++) {
		parent->children[i-1] = parent->children[i];
	}
	parent->children_size--;

	logger(DEBUG, "[unlink] Ended\n");
	return 0;
}

