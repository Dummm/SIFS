#include "../lib/mknod.h"

int sifs_mknod(const char* path, mode_t mode, dev_t dev) {
	logger(DEBUG, "[mknod] Started on path: %s\n", path);
	logger(DEBUG, "[mknod] Mode: %d\t%o\n", mode, mode);

	struct fuse_context* context;
	context = fuse_get_context();

	struct node* root;
	root = (struct node*)context->private_data;

	int last_slash;
	last_slash = (strrchr(path, '/') - path);
	logger(DEBUG, LOG_BOLD LOG_FG_BLUE "[mknod] Position of last slash: %d\n" LOG_RESET, last_slash);

	// Root exception
	if(!last_slash) last_slash++;

	char* parent_path;
	parent_path = malloc((last_slash + 1) * sizeof(char));
	strncpy(parent_path, path, last_slash);
	parent_path[last_slash] = '\0';
	logger(DEBUG, "[mknod] Path of parent node: %s-\n", parent_path);

	struct node* parent;
	parent = get_node_from_path(root, parent_path);
	logger(DEBUG,  "[mknod] Returned node: %s\n", parent->header->name);
	free(parent_path);

	// Creating node
	//struct node *n = malloc(sizeof(struct node));
	struct node *n = calloc(1, sizeof(struct node));
	n->parent = parent;
	n->children = NULL;
	n->children_size = 0;
	n->file = calloc(1, 512);
	strcpy(n->file, "");

	// Adding node metadata
	//n->header = malloc(sizeof(struct tar_header));
	n->header = calloc(1, sizeof(struct tar_header));

	//memcpy(n->header, parent->header, sizeof(struct tar_header));

	n->header->name[0] = '.';
	strcpy(n->header->name + 1, path);
	n->header->name[strlen(path) + 1] = '\0';
	logger(DEBUG, "[mknod] New node name: %s\n", n->header->name);

	sprintf(n->header->mode, "%07o", mode);
	sprintf(n->header->uid, "%07o", getuid());
	sprintf(n->header->gid, "%07o", getgid());
	strcpy(n->header->uname, getpwuid(getuid())->pw_name);
	strcpy(n->header->gname, getgrgid(getgid())->gr_name);
	strcpy(n->header->typeflag, "0");
	strcpy(n->header->size, "0000000000");
	strcpy(n->header->magic, "ustar  ");
	n->header->magic[7] = '\0';

	time_t t;
	t = time(NULL);
 	sprintf(n->header->mtime, "%lo", t);
 	//sprintf(n->header->atime, "%ld", t);
	//sprintf(n->header->ctime, "%ld", t);

	sprintf(n->header->chksum, "%06o", generate_checksum(n->header));
	n->header->chksum[7] = ' ';

	struct node **auxDirChildren;
	parent->children_size++;
	auxDirChildren = realloc(parent->children, parent->children_size * sizeof(struct node*));
	parent->children = auxDirChildren;
	parent->children[parent->children_size - 1] = n;

	logger(DEBUG, "[mknod] Ended\n");
  return 0;
}
