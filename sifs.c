#define FUSE_USE_VERSION 26

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib/tree.h"
#include "lib/tar_structure.h"
#include <fuse.h>

int fd;

// Function that creates tree for directory structure
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

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
static struct options {
	const char *filename;
	const char *contents;
	int show_help;
} options;

#define OPTION(t, p)                           \
	{ t, offsetof(struct options, p), 1 }
	
static const struct fuse_opt option_spec[] = {
	OPTION("--name=%s", filename),
	OPTION("--contents=%s", contents),
	OPTION("-h", show_help),
	OPTION("--help", show_help),
	FUSE_OPT_END
};

void* sifs_init(struct fuse_conn_info* conn, struct fuse_config *fc) {
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

	return root;
}
int sifs_getattr(const char* path, struct stat* sbuf, struct fuse_file_info *ffi) {

	printf("\t%s\n", path);

	return 0;
}
int sifs_open(const char* path, struct fuse_file_info* fi) {
	char *dirname;	// Directory
	char *basename;	// Filename
	int ret;				// Returned value

	ret = -ENOENT;	// Error no entry

	// The position of the last '/'
	int pos;
	pos = strrchr(path, '/') - path;

	dirname = malloc((pos + 2) * sizeof(char));
	strncpy(dirname, path, pos + 1);
	dirname[pos + 1] = '\0';

	basename = malloc((strlen(path) - pos) * sizeof(char));
	strcpy(basename, path + pos + 1);

	printf("\tDirectory: %s\n\tFilename: %s", dirname, basename);

	free(dirname);
	free(basename);

	return ret;
}

static struct fuse_operations sifs_oper = {
	.init 		= sifs_init,
	.getattr 	= sifs_getattr,
	.open 		= sifs_open
};

int main(int argc, char **argv) {

	// Opening file
	if ((fd = open(argv[3], O_RDONLY)) == -1) {
		printf("File open error: %d\n%s\n\n", errno, argv[1]);
		return -1;
	}
	else {
		printf("Opened file: %s\n\n", argv[3]);
	}

	// Moving reading head to beginning of file
	lseek(fd, 0, SEEK_SET);

	/*
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
	*/

	struct fuse_args args = FUSE_ARGS_INIT(argc - 1, argv);

	// Parse options
	//if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1)
	//	return 1;

	int ret;
	ret = fuse_main(args.argc, args.argv, &sifs_oper, NULL);
	fuse_opt_free_args(&args);

	close(fd);

	return ret;
}
