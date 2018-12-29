#include "../lib/readdir.h"

int sifs_readdir(
  const char* path, void* buf, fuse_fill_dir_t filler, off_t offset,
  struct fuse_file_info* fi, enum fuse_readdir_flags flags
) {
	logger(DEBUG, "[readdir] Started on path: %s\n", path);

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
	n = get_node_from_path(root, path);


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
		logger(DEBUG, LOG_BOLD LOG_FG_GREEN "[readdir] Buffer: %s\n" LOG_RESET , buffer);
	}

	logger(DEBUG, "[readdir] Ended\n");
  return 0;
}
