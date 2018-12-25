#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib/tree.h"

int populate_tree_directory(int fd, struct node *dir) {
	struct tar_header *auxTar = malloc(sizeof(struct tar_header));
	struct node *auxNode;
	struct node **auxDirChildren;

	printf("\nCurrent directory node: %s\n", dir->header->name);

	while (read(fd, auxTar, sizeof(struct tar_header))) {
		// Verifying if the file we read is in the same directory
		if (strncmp(
			dir->header->name,
			auxTar->name,
			strlen(dir->header->name) - 1) != 0) {
				free(auxTar);
				break;
		}

		printf("\tRead file\\directory %s\n", auxTar->name);

		// Creating node
		auxNode = malloc(sizeof(struct node));
		auxNode->parent = NULL;
		auxNode->children = NULL;
		auxNode->children_size = 0;
		auxNode->file = NULL;

		auxNode->parent = dir;
		auxNode->header = auxTar;
		printf("\tCreated node.\n");

		// Adding node to parent
		/*
		printf("Previous children:\n");
		int i;
		for (i = 0; i < dir->children_size; i++) {
			printf("\t%p\t%s\n", dir->children[i]->header->name, dir->children[i]->header->name);
		}
		*/
		dir->children_size++;
		auxDirChildren = realloc(dir->children, dir->children_size * sizeof(struct node *));
		dir->children = auxDirChildren;
		dir->children[dir->children_size - 1] = auxNode;
		/*
		printf("New children:\n");
		for (i = 0; i < dir->children_size; i++) {
			printf("\t%p\t%s\n", dir->children[i]->header->name, dir->children[i]->header->name);
		}
		*/
		printf("\tAdded node to parent.\n");

		if (strcmp(auxNode->header->typeflag, "5") == 0) {
			// Node is a directory
			printf("\tAdding children to directory node for %s\n", auxTar->name);
			fd = populate_tree_directory(fd, auxNode);
			printf("\t\tFinished adding children to %s\n", auxTar->name);
		} else {
			// Node is a file
			printf("\tAdding file for %s\n", auxTar->name);
			int sz = strtoul(auxTar->size, NULL, 8);
			auxNode->file = malloc(sz);
			read(fd, auxNode->file, sz);
			//printf("File content[%d]:\n", sz);
			//printf("%s", auxNode->file);

			// Reallinging reading head
			int pos = lseek(fd, 0, SEEK_CUR);
			lseek(fd, 512 - (pos % 512), SEEK_CUR);
		}

		auxTar = malloc(sizeof(struct tar_header));
	}

	// Undo last read
	// Last read is outside the current folder
	lseek(fd, -(sizeof(struct tar_header)), SEEK_CUR);

	//free(auxTar);

	// Returning new reading position
	return fd;
}

// Function that prints tree
void print_tree(struct node *n) {
	printf("%s\n", n->header->name);

	int i;
	for (i = 0; i < n->children_size; i++) {
		print_tree(n->children[i]);
	}
}

int main(int argc, char **argv) {

	// Opening file
	int fd;
	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		printf("File open error: %d\n\n", errno);
		return -1;
	}
	else {
		printf("Opened file: %s\n\n", argv[1]);
	}

	// Moving reading head to beginning of file
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

	// Creating tree for directory structure
	populate_tree_directory(fd, root);

	/// TEST TEST WOOOOOOOOOOOOOO
	printf("\n\n\nTar structure:\n");
	print_tree(root);

	close(fd);
	return 0;
}

