#include "../lib/mkdir.h"

unsigned int generate_checksum(const struct tar_header* h) {
	unsigned int i;
	unsigned char *p = (unsigned char*) h;
	unsigned int res = 256 + 47; // ???
	for (i = 0; i < offsetof(struct tar_header, chksum); i++) {
		res += p[i];
	}
	for (i = offsetof(struct tar_header, typeflag); i < sizeof(struct tar_header); i++) {
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
	//n->header = malloc(sizeof(struct tar_header));
	n->header = calloc(1, sizeof(struct tar_header));

	//memcpy(n->header, parent->header, sizeof(struct tar_header));

	n->header->name[0] = '.';
	strcpy(n->header->name + 1, path);
	n->header->name[strlen(path) + 1] = '/';
	n->header->name[strlen(path) + 2] = '\0';
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] New node name: %s\n" LOG_RESET, n->header->name);

	sprintf(n->header->mode, "%07o", mode);
	sprintf(n->header->uid, "%07o", getuid());
	sprintf(n->header->gid, "%07o", getgid());
	//sprintf(n->header->uid, "%s", getpwuid(getuid())->pw_name);qq
	//sprintf(n->header->gid, "%s", getgrgid(getgid())->gr_name);
	strcpy(n->header->typeflag, "5");
	//sprintf(n->header->size, "%011ld", (long int)10000);
	//printf("%s\t%o\n", n->header->size);
	strcpy(n->header->size, "10000");



	strcpy(n->header->uname, getpwuid(getuid())->pw_name);
	strcpy(n->header->gname, getgrgid(getgid())->gr_name);

	//sprintf(n->header->magic, "%o" "ustar  ");
	//n->header->magic[7] = '\0';
	strcpy(n->header->magic, "ustar  ");
	n->header->magic[7] = '\0';

	time_t t;
	t = time(NULL);
 	sprintf(n->header->mtime, "%lo", t);
 	//sprintf(n->header->atime, "%ld", t);
	//sprintf(n->header->ctime, "%ld", t);

	sprintf(n->header->chksum, "%06o", generate_checksum(n->header));
	n->header->chksum[7] = ' ';

	logger(DEBUG, LOG_BOLD LOG_BG_YELLOW LOG_FG_WHITE);

	logger(DEBUG, "%s\n", parent->header->chksum);
	logger(DEBUG, "%06o\n", generate_checksum(parent->header));

	logger(DEBUG, LOG_RESET);

	struct node **auxDirChildren;
	parent->children_size++;
	auxDirChildren = realloc(parent->children, parent->children_size * sizeof(struct node*));
	parent->children = auxDirChildren;
	parent->children[parent->children_size - 1] = n;

	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Ended\n" LOG_RESET);
  return 0;
}
