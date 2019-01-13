#include "../lib/destroy.h"
int fd; /* gets value from sifs.c */
int fdd;
int enfd;

void print_tree2(struct node *n) {
	//~ logger(DEBUG, "\t%s%d\n", n->header->name, strtoul(n->header->size, NULL, 8));
	if(strcmp(n->header->name, "./") != 0){
		//printf("%s\n", n->header->name);
		//printf("%s\n%06o\n\n", n->header->chksum, generate_checksum(n->header));
		//generate_checksum(n->header);

		if(strcmp(n->header->typeflag, "5") == 0) {
			//n->header->typeflag[-1] = ' ';
			sprintf(n->header->size, "%011ld", (long int)0);
		}

		/*
		if(strcmp(n->header->name, "./directory2/") == 0){
			sprintf(n->header->chksum, "%06o", 0);
		}
		*/

		/*
		printf("%s\n", n->header->name);
		printf("\t%ld\n", sizeof(n->header->name));
		printf("\t%ld\n", sizeof(n->header->mode));
		printf("\t%ld\n", sizeof(n->header->uid));
		printf("\t%ld\n", sizeof(n->header->gid));
		printf("\t%ld\n", sizeof(n->header->size));
		printf("\t%ld\n", sizeof(n->header->mtime));
		printf("\t%ld\n", sizeof(n->header->chksum));
		printf("\t%ld\n", sizeof(n->header->typeflag));
		printf("\t%ld\n", sizeof(n->header->linkname));
		*/

		printf("\tPozitie pentru %s:\t%ld\n", n->header->name, lseek(fd, 0, SEEK_CUR));
		write(fd, n->header, sizeof(struct tar_header));

		if(strcmp(n->header->typeflag, "0") == 0){
			int file_size = strtoul(n->header->size, NULL, 8);
			if(file_size!=0) {

				//write(fdd, n->file, 512 * ((file_size/512) + 1));

				write(fd, n->file, file_size);
				int pos = lseek(fd, 0, SEEK_CUR);
				lseek(fd, (512 - pos % 512), SEEK_CUR);
			}
		}
	}

	int i;
	for (i = 0; i < n->children_size; i++) {
		print_tree2(n->children[i]);
	}

}

void sifs_destroy(void* private_data) {
	lseek(fd, 0, SEEK_SET); /* move to the beginning of file */
		//~ printf("FAILED!!!");
	//~ }
	
	//struct node* parent;
	//parent = get_node_from_path(private_data,"/");
	print_tree2((struct node*)private_data);

	/*
	lseek(fdd, 0, SEEK_END);
	for (int i = 0; i < 18*512 - 10; i++){
		write(fdd, "\0", 1);
	}
	lseek(fdd, 0, SEEK_SET);
	*/

	lseek(fd, 17 * 512 - 1, SEEK_CUR);
	/*
	char *e = calloc(1, sizeof(char));
	e[0] = '\0';
	write(fdd, e, 1);
	*/
	write(fd, "\0", 1);
}

