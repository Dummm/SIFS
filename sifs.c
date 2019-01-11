/**
	Build:
		make
	Run:
		./sifs -f [file-path] [tar-path]
		./sifs -f ./test ../Tars/testTar.tar
*/

//#define FUSE_USE_VERSION 26

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib/destroy.h"
#include "lib/getattr.h"
// #include "lib/fgetattr.h"
// #include "lib/access.h"
// #include "lib/readlink.h"
#include "lib/opendir.h"
#include "lib/readdir.h"
#include "lib/mknod.h"
#include "lib/mkdir.h"
// #include "lib/unlink.h"
// #include "lib/rmdir.h"
// #include "lib/symlink.h"
// #include "lib/rename.h"
// #include "lib/link.h"
// #include "lib/chmod.h"
// #include "lib/chown.h"
// #include "lib/truncate.h"
// #include "lib/ftruncate.h"
// #include "lib/utimens.h"
 #include "lib/open.h"
 #include "lib/read.h"
 #include "lib/write.h"
// #include "lib/statfs.h"
// #include "lib/release.h"
// #include "lib/releasedir.h"
// #include "lib/fsync.h"
// #include "lib/fsyncdir.h"
// #include "lib/flush.h"
// #include "lib/lock.h"

#include "lib/logger.h"
#include "lib/tree.h"
#include "lib/tar_structure.h"
#include <fuse.h>

extern int fd, fdd;
int fd, fdd;
/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
 /*
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
*/

// Function that creates tree for directory structure
int populate_tree_directory(int fd, struct node *dir) {
	struct tar_header *auxTar = malloc(sizeof(struct tar_header));
	struct node *auxNode;
	struct node **auxDirChildren;

	//printf("\nCurrent directory node: %s\n", dir->header->name);

	while (read(fd, auxTar, sizeof(struct tar_header))) {
		/// Verifying if the file we read is in the same directory
		if (strncmp(
		dir->header->name,
		auxTar->name,
		strlen(dir->header->name) - 1) != 0) {
			free(auxTar);
			break;
		}

		// Creating node
		//auxNode = malloc(sizeof(struct node));
		auxNode = calloc(1, sizeof(struct node));
		auxNode->parent = NULL;
		auxNode->children = NULL;
		auxNode->children_size = 0;
		auxNode->file = NULL;

		auxNode->parent = dir;
		auxNode->header = auxTar;
		//printf("\tCreated node.\n");

		// Adding node to parent
		dir->children_size++;
		auxDirChildren = realloc(dir->children, dir->children_size * sizeof(struct node *));
		dir->children = auxDirChildren;
		dir->children[dir->children_size - 1] = auxNode;
		//printf("\tAdded node to parent.\n");

		if (strcmp(auxNode->header->typeflag, "5") == 0) {
			// Node is a directory
			//printf("\tAdding children to directory node for %s\n", auxTar->name);
			sprintf(auxNode->header->size, "%ld", (long int)10000);
			fd = populate_tree_directory(fd, auxNode);
			//printf("\t\tFinished adding children to %s\n", auxTar->name);
		} else {
			// Node is a file
			//printf("\tAdding file for %s\n", auxTar->name);
			int sz = strtoul(auxTar->size, NULL, 8);

			//auxNode->file = malloc(sz);
			auxNode->file = calloc(1, sz);

			read(fd, auxNode->file, sz);

			// Reallinging reading head
			if(sz) {
				int pos = lseek(fd, 0, SEEK_CUR);
				lseek(fd, 512 - (pos % 512), SEEK_CUR);
			}
		}

		//auxTar = malloc(sizeof(struct tar_header));
		auxTar = calloc(1, sizeof(struct tar_header));
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
	logger(DEBUG, "\t%s\n", n->header->name);

	int i;
	for (i = 0; i < n->children_size; i++) {
		print_tree(n->children[i]);
	}
}

void* sifs_init(struct fuse_conn_info* conn, struct fuse_config* cfg) {
  logger(DEBUG, "[init] Started\n");

	// Creating root
	struct node *root = malloc(sizeof(struct node));
	root->parent = NULL;
	root->children = NULL;
	root->children_size = 0;
	root->file = NULL;

	// Adding root folder path
	struct stat s;
	fstat(fd, &s);

	root->header = malloc(sizeof(struct tar_header));
	strcpy(root->header->name, "./");
	/*
	sprintf(root->header->mode, "%u", s.st_mode);
	sprintf(root->header->uid, "%u", s.st_uid);
	sprintf(root->header->gid, "%u", s.st_gid);
	strcpy(root->header->chksum, "00000000");
	strcpy(root->header->typeflag, "5");
	sprintf(root->header->size, "%ld", s.st_size);
 	sprintf(root->header->mtime, "%ld", s.st_mtime);
 	sprintf(root->header->atime, "%ld", s.st_atime);
	sprintf(root->header->ctime, "%ld", s.st_ctime);
	*/

	sprintf(root->header->mode, "%07o", S_IRWXU  | S_IRWXG | S_IRWXO);
	sprintf(root->header->uid, "%07o", getuid());
	sprintf(root->header->gid, "%07o", getgid());
	strcpy(root->header->typeflag, "5");
	//sprintf(root->header->size, "%011ld", (long int)0);
	//strcpy(root->header->size, "10000");
	sprintf(root->header->size, "%ld", (long int)10000);

	//sprintf(root->header->uname, "%s", getpwuid(getuid())->pw_name);
	//sprintf(root->header->gname, "%s", getgrgid(getgid())->gr_name);
	strcpy(root->header->uname, getpwuid(getuid())->pw_name);
	strcpy(root->header->gname, getgrgid(getgid())->gr_name);

	//sprintf(root->header->magic, "%o" "ustar  ");
	//root->header->magic[7] = '\0';
	strcpy(root->header->magic, "ustar  ");
	root->header->magic[7] = '\0';

	time_t t;
	t = time(NULL);
 	sprintf(root->header->mtime, "%lo", t);
 	//sprintf(n->header->atime, "%ld", t);
	//sprintf(n->header->ctime, "%ld", t);

	sprintf(root->header->chksum, "%06o", generate_checksum(root->header));
	root->header->chksum[7] = ' ';

	// Creating tree for directory structure
	populate_tree_directory(fd, root);
	logger(DEBUG, "[init] Created directory tree\n");

	logger(DEBUG, "[init] Tar directory structure:\n");
	print_tree(root);

  logger(DEBUG, "[init] Ended\n");

	// Tree will be memorized in the context (fuse_get_context)
	return root;
}

static struct fuse_operations sifs_oper = {
  .init 				= sifs_init,
  .destroy 		= sifs_destroy,
  .getattr 			= sifs_getattr,
  // .fgetattr 		= sifs_fgetattr,
  // .access 			= sifs_access,
  // .readlink 		= sifs_readlink,
  .opendir 			= sifs_opendir,
  .readdir 			= sifs_readdir,
  .mknod 			= sifs_mknod,
  .mkdir 				= sifs_mkdir,
  // .unlink 			= sifs_unlink,
  // .rmdir 			= sifs_rmdir,
  // .symlink 		= sifs_symlink,
  // .rename 			= sifs_rename,
  // .link 				= sifs_link,
  // .chmod 			= sifs_chmod,
  // .chown 			= sifs_chown,
  // .truncate 		= sifs_truncate,
  // .ftruncate 	= sifs_ftruncate,
  // .utimens 		= sifs_utimens,
  .open 				= sifs_open,
  .read 				= sifs_read,
 .write 			= sifs_write
  // .statfs 			= sifs_statfs,
  // .release 		= sifs_release,
  // .releasedir 	= sifs_releasedir,
  // .fsync 			= sifs_fsync,
  // .fsyncdir 		= sifs_fsyncdir,
  // .flush 			= sifs_flush,
  // .lock 				= sifs_lock,
};

int main(int argc, char **argv) {
  set_log_level(DEBUG);
  set_log_output(stdout);

	// Opening file
	if ((fd = open(argv[argc - 1], O_RDWR)) == -1) {
		logger(DEBUG, "[main] File open error(%s): %d\n", argv[argc - 1], errno);
		return -1;
	}
	else {
		logger(DEBUG, "[main] Opened file: %s\n", argv[argc - 1]);
	}
	if ((fdd = open("testy.tar", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
			logger(DEBUG, "[main] File2 open error(%s): %d\n", "testy.tar", errno);
			return -1;
		}
		else {
			logger(DEBUG, "[main] Opened file2: %s\n", "testy.tar");
		}
	// Moving reading head to beginning of file
	lseek(fd, 0, SEEK_SET);

	struct fuse_args args = FUSE_ARGS_INIT(argc - 1, argv);

	// Parse options
	//if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1)
	//	return 1;

	int ret;
	ret = fuse_main(args.argc, args.argv, &sifs_oper, NULL);
	fuse_opt_free_args(&args);

	close(fd);
	close(fdd);

	logger(DEBUG, "[main] Ended\n");

	return ret;
}
