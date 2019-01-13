#include "../lib/write.h"

int sifs_write(const char* path, const char *buf, size_t size, off_t offset, struct fuse_file_info* fi) {
	logger(DEBUG, "[write] Started on path: %s\n", path);
	struct fuse_context* context;
	context=fuse_get_context();

	struct node*root;
	root=(struct node*)context->private_data;

	struct node*n;
	n=get_node_from_path(root,path);

	n->file = realloc(n->file, strlen(buf));

	memcpy(n->file + offset, buf, size);

	sprintf(n->header->size, "%011lo", strlen(n->file));
	printf("%s\n", n->header->size);

	sprintf(n->header->chksum, "%06o", generate_checksum(n->header));
	n->header->chksum[7] = ' ';

	logger(DEBUG,  "[write] Ended\n");
	logger(DEBUG, LOG_RESET);
  return size;
}

