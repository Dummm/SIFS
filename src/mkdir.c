#include "../lib/mkdir.h"

struct node* get_node_from_path3(struct node* n, const char* path) {
	// +1 to remove . from filename

	struct node* aux;
	aux = n;

	// Root
	if (strcmp(aux->header->name + 1, path) == 0) {
		logger(DEBUG, "[mkdir/get_node_from_path] Node found: %s\n", aux->header->name);
		return aux;
	}

	int modified = 0;
	while (
	(aux->header->name[strlen(aux->header->name) - 1] == '/') ?
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1) - 1) == 0) :
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1)) == 0)
	) {
		logger(DEBUG, "[mkdir/get_node_from_path] Current node: %s\n", aux->header->name + 1);
		logger(DEBUG, "[mkdir/get_node_from_path] Children: %d\n", aux->children_size);

		int i;
		for(i = 0; i < aux->children_size; i++) {
			logger(DEBUG, "[mkdir/get_node_from_path] \tTrying node: %s\n", aux->children[i]->header->name + 1);
			if (
			(aux->children[i]->header->name[strlen(aux->children[i]->header->name) - 1] == '/') ?
			(strncmp(aux->children[i]->header->name + 1, path, strlen(aux->children[i]->header->name + 1) - 1) == 0) :
			(strncmp(aux->children[i]->header->name + 1, path, strlen(aux->children[i]->header->name + 1)) == 0)
			) {
				aux = aux->children[i];
				modified = 1;
				break;
			}
		}
		if(!modified) break;
		else modified = 0;
	}

	/*
	if ((strcmp(aux->header->name, n->header->name) == 0) && !modified) {
		logger(DEBUG, "[getattr/get_node_from_path] Node not found\n");
		return NULL;
	}
	*/

	/*
	if (
	(aux->header->name[strlen(aux->header->name) - 1] == '/') ?
	(strncmp(aux->header->name + 1, path, strlen(path)) == 0) :
	(strncmp(aux->header->name + 1, path, strlen(path)) == 0)
	) {
	*/
	if (strncmp(aux->header->name + 1, path, strlen(path)) == 0) {
		logger(DEBUG, "[mkdir/get_node_from_path] Node found: %s\n", aux->header->name);
		return aux;
	}

	logger(DEBUG, "[mkdir/get_node_from_path] Node not found\n");
	return NULL;
}

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
	parent = get_node_from_path3(root, parent_path);
	logger(DEBUG, LOG_BOLD LOG_FG_RED "[mkdir] Returned node: %s\n" LOG_RESET, parent->header->name);

	/*
	// Creating node
	struct node *n = malloc(sizeof(struct node));
	n->parent = parent;
	n->children = NULL;
	n->children_size = 0;
	n->file = NULL;

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
