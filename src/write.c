#include "../lib/write.h"

int sifs_write(
    const char* path,
    const char *buf,
    size_t size,
    off_t offset,
    struct fuse_file_info* fi
) {
	//logger(ERROR, "UNIMPLEMENTED: write, path: %s\n", path);
	struct fuse_context* context;
	context=fuse_get_context();
	
	
	struct node*root;
	root=(struct node*)context->private_data;
	struct node*n;
	n=get_node_from_path(root,path);
	
	
	size_t file_length = strlen(buf);
	if (offset < file_length) {
		if (offset + size > file_length)
			size = file_length-offset;
		logger(DEBUG, "[wrote] %s\n\n\n", buf);
		memcpy(buf + offset, buf, size);
	} else {
		return -1;
	}

  return size;
}

