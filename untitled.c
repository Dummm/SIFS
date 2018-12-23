#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include "tar_structure.h"
#include "tree.c"

int populate_tree_directory(int fd, struct node *dir) {
	struct tar_header *auxTar = malloc(sizeof(struct tar_header));
	struct node *auxNode;
	struct node **auxDirChildren;

	printf("\nCurrent directory node: %s\n", dir->header->name);

	while (read(fd, auxTar, sizeof(struct tar_header))) {
		// Verify if the file we read is in the same directory
		if (strncmp(
			dir->header->name,
			auxTar->name,
			strlen(dir->header->name) - 1) != 0) break;

		printf("Read file\\directory %s\n", auxTar->name);

		// Create node
		//node_init(auxNode);
		auxNode = malloc(sizeof(struct node));
		//auxNode->header = malloc(sizeof(struct tar_header));
		auxNode->parent = NULL;
		auxNode->children = NULL;
		auxNode->children_size = 0;
		auxNode->file = NULL;

		auxNode->parent = dir;
		auxNode->header = auxTar;
		printf("\tCreated node.\n");

		// Add node to parent
		dir->children_size++;
		auxDirChildren = realloc(dir->children, dir->children_size * sizeof(struct node *));
		dir->children = auxDirChildren;
		dir->children[dir->children_size - 1] = auxNode;
		printf("\tAdded node to parent.\n");

		if (strcmp(auxNode->header->typeflag, "5") == 0) {
			// Node is a directory
			printf("Creating directory node for %s\n", auxTar->name);
			fd = populate_tree_directory(fd, auxNode);
			printf("Created directory node for %s\n", auxTar->name);
		} else {
			// Node is a file
			printf("Creating file node for %s\n", auxTar->name);
			int sz = strtoul(auxTar->size, NULL, 8);
			auxNode->file = malloc(sz);
			read(fd, auxNode->file, sz);
			//printf("File content[%d]:\n", sz);
			//printf("%s", auxNode->file);

			int pos = lseek(fd, 0, SEEK_CUR);
			//printf("%d", 512 - (pos % 512));
			lseek(fd, 512 - (pos % 512), SEEK_CUR);
		}
	}

	// PROBABIL NU AR TREBUI SA FIE IN WHILE, BOSS
	// Undo last read
	lseek(fd, -(sizeof(struct tar_header)), SEEK_CUR);

	//free(auxTar);

	// Return new reading position
	return fd;
}

void print_tree(struct node *n) {
	printf("%s\n", n->header->name);

	int i;
	for(i = 0; i < n->children_size; i++) {
		print_tree(n->children[i]);
	}
}

int main(int argc, char **argv) {

	int fd;
	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		printf("File open error: %d\n\n", errno);
		return -1;
	}
	else
		printf("Opened file: %s\n\n", argv[1]);

	lseek(fd, 0, SEEK_SET);

	struct node *root = malloc(sizeof(struct node));
	node_init(root);

	// Adding root folder path
	root->header = malloc(sizeof(struct tar_header));
	strcpy(root->header->name, "./");

	populate_tree_directory(fd, root);


	/// TEST TEST WOOOOOOOOOOOOOO
	printf("\n\n\n");
	print_tree(root);

	close(fd);
	return 0;
}

