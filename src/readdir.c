#include "../lib/readdir.h"

int sifs_readdir(
  const char* path, void* buf, fuse_fill_dir_t filler, off_t offset,
  struct fuse_file_info* fi, enum fuse_readdir_flags flags
) {
	logger(DEBUG, "[readdir] Started on path: %s\n", path);

	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);

	struct fuse_context* context;
	context = fuse_get_context();

	struct node* root;
	root = (struct node*)context->private_data;

	struct node *n;
	n = get_node_from_path(root, path);

	for(int i = 0; i < n->children_size; i++){
		char buffer[50];
		strcpy(buffer, n->children[i]->header->name+2);
		// delete the '/' at the end of directories (why?)
		if( strcmp(n->children[i]->header->typeflag, "5") == 0){
			buffer[ strlen(buffer) - 1 ] = '\0';
		}
		if(strrchr(buffer, '/')) {
			filler(buf, strrchr(buffer, '/') + 1, NULL, 0, 0);
		}
		else {
			filler(buf, buffer, NULL, 0, 0);
		}
		logger(DEBUG, "[readdir] Buffer: %s\n" , buffer);
	}

	logger(DEBUG, "[readdir] Ended\n");
  return 0;
}
