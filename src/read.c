#include "../lib/read.h"

int sifs_read(const char* path, char *buf, size_t size, off_t offset, struct fuse_file_info* fi) {
	// Get to current file node:
	struct fuse_context* context;
	context = fuse_get_context();

	struct node* root;
	root = (struct node*)context->private_data;

	struct node* n;
	n = get_node_from_path(root, path);


	size_t file_length = strlen( (char*)n->file );
	if (offset < file_length) {
		if (offset + size > file_length) 
			size = file_length - offset;
		printf("%s\n\n\n", n->file + offset);
		memcpy(buf, (char*)n->file + offset, size);
	} else {
		size = 0;
	}
	
	return size;
}

