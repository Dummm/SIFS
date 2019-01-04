#include "../lib/mkdir.h"

unsigned int generate_checksum(const struct tar_header* h) {
	unsigned int i;
	unsigned char *p = (unsigned char*) h;
	unsigned int res = 256;
	for (i = 0; i < offsetof(struct tar_header, chksum); i++) {
		res += p[i];
	}
	for (i = offsetof(struct tar_header, typeflag); i < sizeof(*h); i++) {
		res += p[i];
	}
	return res;
}

int sifs_mkdir(const char* path, mode_t mode) {
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Started on path: %s\n" LOG_RESET, path);
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Mode: %d\t%o\n" LOG_RESET, mode, mode);

	struct fuse_context* context;
	context = fuse_get_context();

	struct node* root;
	root = (struct node*)context->private_data;

	int last_slash;
	last_slash = (strrchr(path, '/') - path);
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Position of last slash: %d\n" LOG_RESET, last_slash);

	// Root exception
	if(!last_slash) last_slash++;

	char* parent_path;
	parent_path = malloc((last_slash + 1) * sizeof(char));
	strncpy(parent_path, path, last_slash);
	parent_path[last_slash] = '\0';
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Path of parent node: %s-\n" LOG_RESET, parent_path);

	struct node* parent;
	parent = get_node_from_path(root, parent_path);
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Returned node: %s\n" LOG_RESET, parent->header->name);
	free(parent_path);

	// Creating node
	struct node *n = malloc(sizeof(struct node));
	n->parent = parent;
	n->children = NULL;
	n->children_size = 0;
	n->file = NULL;

	// Adding node metadata
	n->header = malloc(sizeof(struct tar_header));

	n->header->name[0] = '.';
	strcpy(n->header->name + 1, path);
	n->header->name[strlen(path) + 1] = '/';
	n->header->name[strlen(path) + 2] = '\0';
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] New node name: %s\n" LOG_RESET, n->header->name);

	sprintf(n->header->mode, "%u", mode | S_IFDIR);
	sprintf(n->header->uid, "%u", getuid());
	sprintf(n->header->gid, "%u", getgid());
	strcpy(n->header->typeflag, "5");
	sprintf(n->header->size, "%d", 0);

	time_t t;
	t = time(NULL);
 	sprintf(n->header->mtime, "%ld", t);
 	sprintf(n->header->atime, "%ld", t);
	sprintf(n->header->ctime, "%ld", t);

	sprintf(n->header->chksum, "%d", generate_checksum(n->header));

	struct node **auxDirChildren;
	parent->children_size++;
	auxDirChildren = realloc(parent->children, parent->children_size * sizeof(struct node*));
	parent->children = auxDirChildren;
	parent->children[parent->children_size - 1] = n;

	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Ended\n" LOG_RESET);
  return 0;
}
