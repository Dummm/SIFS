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
	printf("vreau sa mor\n");

	printf("%d %d", fd, (int)lseek(fd, 0, SEEK_CUR));

	printf("vreau sa mor\n");
	while (read(fd, auxTar, sizeof(struct tar_header))) {
	printf("impuscat c-un mic pistol\n");
		// Verify if we're in the same directory
		printf("%s\n%s\n", dir->header->name, auxTar->name);
		if (strncmp(dir->header->name, auxTar->name, strlen(dir->header->name) - 1) != 0) break;

		printf("Read %s\n", auxTar->name);
		// Create node
		node_init(auxNode);
		auxNode->parent = dir;
		auxNode->header = auxTar;

		// Add node to parent
		dir->children_size++;
		auxDirChildren = realloc(dir->children, dir->children_size * sizeof(struct node *));
		dir->children = auxDirChildren;
		dir->children[dir->children_size - 1] = auxNode;

		if (strcmp(auxTar->typeflag, "5") == 0) {
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

		// Undo last read
		lseek(fd, -(sizeof(struct tar_header)), SEEK_CUR);
	}

	free(auxTar);

	// Return new reading position
	return fd;
}

int populate_node_tree(int fd, struct node *root) {
	node_init(root);

	struct tar_header *auxTar = malloc(sizeof(struct tar_header));

	while (read(fd, auxTar, sizeof(struct tar_header))) {
		if (auxTar->name[0] == '\0') break;
		/*
		printf("%d\n", lseek(fd, 0, SEEK_CUR));

		printf("%s\n", auxTar->name);
		printf("\tFile mode: %s\n", auxTar->mode);
		printf("\tOwner ID: %s\n", auxTar->uid);
		printf("\tGroup ID: %s\n", auxTar->gid);
		printf("\tSize: %s\n", auxTar->size);
		printf("\tModification time: %s\n", auxTar->mtime);
		printf("\tChecksum: %s\n", auxTar->chksum);
		printf("\tType flag: %s\n", auxTar->typeflag);
		printf("\tLink name: %s\n", auxTar->linkname);

		/// UStar
		printf("\tUStar: %s\n", auxTar->magic);
		printf("\tUStar version: %s\n", auxTar->version);
		printf("\tOwner name: %s\n", auxTar->uname);
		printf("\tGroup name: %s\n", auxTar->gname);
		printf("\tDevice major: %s\n", auxTar->devmajor);
		printf("\tDevice minor: %s\n", auxTar->devminor);
		printf("\tPrefix: %s\n", auxTar->prefix);
		//printf("\t: %s\n", auxTar->fill2);
		//printf("\t: %s\n", auxTar->fill3);
		printf("\tIs extended: %s\n", auxTar->isextended);
		//printf("\t: %s\n", auxTar->sparse);
		printf("\tReal size: %s\n", auxTar->realsize);
		printf("\tOffset: %s\n", auxTar->offset);
		printf("\tAccess time: %s\n", auxTar->atime);
		printf("\tCreation Time: %s\n", auxTar->ctime);
		//printf("\t: %s\n", auxTar->mfill);
		printf("\tX magic: %s\n", auxTar->xmagic);
		*/

		if (strcmp(auxTar->typeflag, "5") == 0) {
			int sz = strtoul(auxTar->size, NULL, 8);
			char *fileContent = malloc(sz);
			read(fd, fileContent, sz);
			printf("File content[%d]:\n", sz);
			printf("%s", fileContent);
			free(fileContent);

			int pos = lseek(fd, 0, SEEK_CUR);
			//printf("%d", 512 - (pos % 512));
			lseek(fd, 512 - (pos % 512), SEEK_CUR);
		} else {
			int sz = strtoul(auxTar->size, NULL, 8);
			char *fileContent = malloc(sz);
			read(fd, fileContent, sz);
			printf("File content[%d]:\n", sz);
			printf("%s", fileContent);
			free(fileContent);

			int pos = lseek(fd, 0, SEEK_CUR);
			//printf("%d", 512 - (pos % 512));
			lseek(fd, 512 - (pos % 512), SEEK_CUR);
		}

		printf("\n");
	}


	printf("%d\n\n", (int)lseek(fd, 0, SEEK_END));


	free(auxTar);
	return 1;
}

int main(int argc, char **argv) {

	printf("muie\n");
	int fd;
	//if ((fd = open(argv[1], O_RDONLY)) == -1)
	if ((fd = open("testTar.tar", O_RDONLY)) == -1)
		printf("File open error: %d\n", errno);
	printf("muie\n");
	lseek(fd, 0, SEEK_SET);

	printf("muie\n");
	struct node *root;
	node_init(root);
	printf("muie?\n");

	printf("%o", root->header);
	printf("muie.\n");
	//char tmp[3] = "./";
	//strcpy(root->header->name, tmp);
	struct tar_header a;
	strcpy(a.name, "./");

	*(root->header) = a;

	printf("muie!\n");
	populate_tree_directory(fd, root);

	close(fd);
	return 0;
}

