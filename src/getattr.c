#include "../lib/getattr.h"
/*
	./sifs -f -d -o default_permissions ./f ../Tars/testTar.tar
	./sifs -f -d -o default_permissions ../tests/testFolder ../tests/tars/testTar.tar

*/
struct node* get_node_from_path(struct node* n, const char* path) {
	// +1 to remove . from filename

	struct node* aux;
	aux = n;

	// Root
	if (strcmp(aux->header->name + 1, path) == 0) {
		logger(DEBUG, "[getattr/get_node_from_path] Node found: %s\n", aux->header->name);
		return aux;
	}

	int modified = 0;
	while (
	(aux->header->name[strlen(aux->header->name) - 1] == '/') ?
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1) - 1) == 0) :
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1)) == 0)
	) {
		logger(DEBUG, "[getattr/get_node_from_path] Current node: %s\n", aux->header->name + 1);
		logger(DEBUG, "[getattr/get_node_from_path] Children: %d\n", aux->children_size);

		int i;
		for(i = 0; i < aux->children_size; i++) {
			logger(DEBUG, "[getattr/get_node_from_path] \tTrying node: %s\n", aux->children[i]->header->name + 1);
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

	if ((strcmp(aux->header->name, n->header->name) == 0) && !modified) {
		logger(DEBUG, "[getattr/get_node_from_path] Node not found\n");
		return NULL;
	}

	if (
	(aux->header->name[strlen(aux->header->name) - 1] == '/') ?
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1) - 1) == 0) :
	(strncmp(aux->header->name + 1, path, strlen(aux->header->name + 1)) == 0)
	) {
		logger(DEBUG, "[getattr/get_node_from_path] Node found: %s\n", aux->header->name);
		return aux;
	}

	logger(DEBUG, "[getattr/get_node_from_path] Node not found\n");
	return NULL;
}

int sifs_getattr(const char* path, struct stat* sbuf, struct fuse_file_info* fi) {
  logger(DEBUG, "[getattr] Started on path: %s\n", path);
	/*
	char *aux;
	aux = malloc(strlen(path) + 2);
	aux[0] = '.'; aux[1] = '\0';
	strcpy(aux + 1, path);
	stat(aux, sbuf);
	//return 0;
	*/

	struct fuse_context* context;
	context = fuse_get_context();

	struct node* root;
	root = (struct node*)context->private_data;

	struct node* n;
	n = get_node_from_path(root, path);
	if (n == NULL) return -ENOENT;
	logger(DEBUG, "[getattr] \tNode returned: %s\n", n->header->name);

	sbuf->st_dev = 0;
	sbuf->st_rdev = 0;
	sbuf->st_blocks = 0;
	// The 'st_dev' and 'st_blksize' fields are ignored.
	// The 'st_ino' field is ignored except if the 'use_ino' mount option is given
	//sbuf->st_mode = strtoul(n->header->mode, NULL, 10);
	//sbuf->st_mode ^= 1UL << 15;
	sbuf->st_mode = ((strtoul(n->header->typeflag, NULL, 10) == 5) ? S_IFDIR : S_IFREG) | S_IRWXU  | S_IRWXG | S_IRWXO;
	//logger(DEBUG, "[getattr] mode: %o\n", sbuf->st_mode);
	//logger(DEBUG, "[getattr] mode: %o\n", S_IFDIR | S_IRWXU  | S_IRWXG | S_IRWXO );

	sbuf->st_nlink		= n->children_size;/* number of hard links */
	sbuf->st_uid			= strtoul(n->header->uid, NULL, 10);    /* user ID of owner */

	sbuf->st_gid			= strtoul(n->header->gid, NULL, 10);    /* group ID of owner */
	sbuf->st_size			= strtol(n->header->size, NULL, 10);    /* total size, in bytes */
	sbuf->st_blksize = 4096; 		/* blocksize for file system I/O */
	sbuf->st_atime		= strtol(n->header->atime, NULL, 10);   	/* time of last access */
	sbuf->st_mtime		= strtol(n->header->mtime, NULL, 10);   	/* time of last modification */
	sbuf->st_ctime		= strtol(n->header->ctime, NULL, 10);   	/* time of last status change */

  logger(DEBUG, "[getattr] Ended\n");
  return 0;
}

