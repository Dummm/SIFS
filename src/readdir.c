#include "../lib/readdir.h"

int sifs_readdir(
  const char* path, void* buf, fuse_fill_dir_t filler, off_t offset,
  struct fuse_file_info* fi
) {
	if (strcmp(path, "/") != 0)
		return -1;
		
	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);
	struct fuse_context* context;
	context = fuse_get_context();

	struct node* root;
	root = (struct node*)context->private_data;
	//filler(buf, "...", NULL, 0,0);
	for(int i = 0; i < root->children_size; i++){
		char buffer[50];
		strcpy(buffer, root->children[i]->header->name+2);
		// delete the '/' at the end of directories (why?)
		if( strcmp(root->children[i]->header->typeflag, "5") == 0){
			buffer[ strlen(buffer) -1 ] = '\0';
		}
		filler(buf, buffer, NULL, 0, 0);
		logger(DEBUG,buffer);
	}
  return 0;
}
