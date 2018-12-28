#include "../lib/mkdir.h"

int sifs_mkdir(const char* path, mode_t mode) {
  logger(
    ERROR,
    "UNIMPLEMENTED: mkdir, path: %s, mode: %lo\n",
    path,
    (unsigned long) mode
  );
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Started on path: %s\n" LOG_RESET, path);
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Mode: %d\t%o\n" LOG_RESET, mode, mode);

	struct fuse_context* context;
	context = fuse_get_context();

	struct node* root;
	root = (struct node*)context->private_data;

	int last_slash;
	last_slash = (strrchr(path, '/') - path);
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Position of last slash: %d\n" LOG_RESET, last_slash);

	char* parent_path;
	parent_path = malloc((last_slash + 2) * sizeof(char));
	strncpy(parent_path, path, last_slash + 1);
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Path of parent node: %s\n" LOG_RESET, parent_path);

	struct node* parent;
	parent = get_node_from_path(root, parent_path);
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Returned node: %s\n" LOG_RESET, parent->header->name);

	// Creating node
	struct node *n = malloc(sizeof(struct node));
	n->parent = parent;
	n->children = NULL;
	n->children_size = 0;
	n->file = NULL;

	/*
	// Adding root folder path
	struct stat s;
	fstat(fd, &s);

	n->header = malloc(sizeof(struct tar_header));
	strcpy(n->header->name, "./");
	sprintf(n->header->mode, "%u", s.st_mode);
	sprintf(n->header->uid, "%u", s.st_uid);
	sprintf(n->header->gid, "%u", s.st_gid);
	strcpy(n->header->chksum, "00000000");
	strcpy(n->header->typeflag, "5");
	sprintf(n->header->size, "%ld", s.st_size);
 	sprintf(n->header->mtime, "%ld", s.st_mtime);
 	sprintf(n->header->atime, "%ld", s.st_atime);
	sprintf(n->header->ctime, "%ld", s.st_ctime);
	*/
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Ended\n" LOG_RESET);
  return 0;
}
