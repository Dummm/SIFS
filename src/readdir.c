#include "../lib/readdir.h"

struct node* get_node_from_path2(struct node* n, const char* path) {
	// +1 to remove . from filename

	struct node* aux;
	aux = n;

	// Root
	if (strcmp(aux->header->name + 1, path) == 0) {
		logger(DEBUG, "[readdir/get_node_from_path] Node found: %s\n", aux->header->name);
		return aux;
	}

	while (
	(aux->header->name[strlen(aux->header->name) - 1] == '/') ?
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1) - 1) == 0) :
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1)) == 0)
	) {
		logger(DEBUG, "[readdir/get_node_from_path] Current node: %s\n", aux->header->name + 1);
		logger(DEBUG, "[readdir/get_node_from_path] Children: %d\n", aux->children_size);

		int i;
		for(i = 0; i < aux->children_size; i++) {
			logger(DEBUG, "[readdir/get_node_from_path] \tTrying node: %s\n", aux->children[i]->header->name + 1);
			if (
			(aux->children[i]->header->name[strlen(aux->children[i]->header->name) - 1] == '/') ?
			(strncmp(aux->children[i]->header->name + 1, path, strlen(aux->children[i]->header->name + 1) - 1) == 0) :
			(strncmp(aux->children[i]->header->name + 1, path, strlen(aux->children[i]->header->name + 1)) == 0)
			) {
				aux = aux->children[i];
				break;
			}
		}
		break;
	}

	if (strcmp(aux->header->name, n->header->name) == 0) {
		logger(DEBUG, "[readdir/get_node_from_path] Node not found\n");
		return NULL;
	}

	if (
	(aux->header->name[strlen(aux->header->name) - 1] == '/') ?
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1) - 1) == 0) :
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1)) == 0)
	) {
		logger(DEBUG, "[readdir/get_node_from_path] Node found: %s\n", aux->header->name);
		return aux;
	}

	logger(DEBUG, "[readdir/get_node_from_path] Node not found\n");
	return NULL;
}

int sifs_readdir(
  const char* path, void* buf, fuse_fill_dir_t filler, off_t offset,
  struct fuse_file_info* fi, enum fuse_readdir_flags flags
) {
	logger(DEBUG, "\t\t[readdir] Started on path: %s\n", path);

	/*
	if (strcmp(path, "/") != 0) {
		logger(DEBUG, "[readdir] Ended\n");
		return -1;
	}
	*/

	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);

	struct fuse_context* context;
	context = fuse_get_context();

	struct node* root;
	root = (struct node*)context->private_data;

	struct node *n;
	n = get_node_from_path2(root, path);


	//filler(buf, "...", NULL, 0,0);
	for(int i = 0; i < n->children_size; i++){
		char buffer[50];
		strcpy(buffer, n->children[i]->header->name+2);
		// delete the '/' at the end of directories (why?)
		if( strcmp(n->children[i]->header->typeflag, "5") == 0){
			buffer[ strlen(buffer) - 1 ] = '\0';
		}
		//filler(buf, buffer, NULL, 0, 0);
		//logger(DEBUG, "\t[readdit] Ce se intampla, doctore? %s\n", strrchr(buffer, '/'));
		if(strrchr(buffer, '/')) {
			filler(buf, strrchr(buffer, '/') + 1, NULL, 0, 0);
		}
		else {
			filler(buf, buffer, NULL, 0, 0);
		}
		logger(DEBUG,"\t[readdir] Buffer: %s\n", buffer);
	}

	logger(DEBUG, "\t[readdir] Ended\n");
  return 0;
}
