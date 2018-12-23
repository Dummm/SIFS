#include "opendir.h"
#include "tree.h"
#include "tar_structure.h"
#include <string.h>
#include <stdlib.h>


int o_opendir(const char* path, struct fuse_file_info* fi) {
	int fd;
	if ((fd = open(path, O_RDONLY)) == -1) {
		printf("File open error: %d\n\n", -1);
		return -1;
	}
	else {
		printf("Opened file: %s\n\n", path);
	}
	lseek(fd, 0, SEEK_SET);

	// Creating root
	struct node *root = malloc(sizeof(struct node));
	root->parent = NULL;
	root->children = NULL;
	root->children_size = 0;
	root->file = NULL;

	// Adding root folder path
	root->header = malloc(sizeof(struct tar_header));
	strcpy(root->header->name, "./");
	return -1;
}
