#include "access.h"
#include "tree.h"
#include "tar_structure.h"
#include "init.h"
void print_tree(struct node *n) {
	printf("%s\n", n->header->name);
	int i;
	for (i = 0; i < n->children_size; i++) {
		print_tree(n->children[i]);
	}
}
int o_access(const char* path, int mode) {
	print_tree(root);
	return 0;
}
