#include "../lib/destroy.h"
int fd; /* gets value from sifs.c */
int fdd;
int enfd;
void print_tree2(struct node *n) {
	logger(DEBUG, "\t%s\n", n->header->name);
	if(strcmp(n->header->name, "./") != 0){
		write(fdd, n->header, sizeof(struct tar_header));
		if(strcmp(n->header->typeflag, "0")==0)
			write(fdd, n->file, strtoul(n->header->size,NULL,8));
	}

	int i;
	for (i = 0; i < n->children_size; i++) {
		print_tree2(n->children[i]);
	}

}

void sifs_destroy(void* private_data) {
	//lseek(fd, 0, SEEK_SET); /* move to the beginning of file */
		//~ printf("FAILED!!!");
	//~ }
	struct node* parent;
	parent = get_node_from_path(private_data,"/");
	print_tree2(parent);
}

