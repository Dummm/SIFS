#include "../lib/write.h"

int sifs_write(const char* path, const char *buf, size_t size, off_t offset, struct fuse_file_info* fi) {
	logger(DEBUG, LOG_BOLD LOG_BG_YELLOW LOG_FG_BLACK);
	logger(DEBUG, "[write] Started on path: %s\n", path);
	struct fuse_context* context;
	context=fuse_get_context();

	struct node*root;
	root=(struct node*)context->private_data;

	struct node*n;
	n=get_node_from_path(root,path);

	n->file = realloc(n->file, strlen(buf));
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n%s", buf);
	//~ size_t file_length = strlen(buf);
	//~ if (offset < file_length) {
		//~ if (offset + size > file_length)
			//~ size = file_length-offset;
		//~ logger(DEBUG, "[wrote] %s\n\n\n", buf);
		memcpy(n->file + offset, buf, size);
	//~ } else {
		//~ return -1;
	//~ }
	logger(DEBUG,  "[write] size: %s\n", n->header->size);
	sprintf(n->header->size, "%lo", strlen(buf));
	logger(DEBUG,  "[write] size: %s\n", n->header->size);

	logger(DEBUG,  "[write] Ended\n");
	logger(DEBUG, LOG_RESET);
  return size;
}

