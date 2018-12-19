struct node {
	struct tar_header *header;
	struct node *parent;
	struct node **children;
	int children_size;
	void *file;
};

int node_init(struct node *node);
int tree_add_child(struct node *node, struct node *new_node);
int tree_remove_node(struct node *);