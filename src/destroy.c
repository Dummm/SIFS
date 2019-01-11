#include "../lib/destroy.h"
int fd; /* gets value from sifs.c */
int fdd;
int enfd;


unsigned int generate_checksum_wew(const struct tar_header* h) {
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

void print_tree2(struct node *n) {
	//~ logger(DEBUG, "\t%s%d\n", n->header->name, strtoul(n->header->size, NULL, 8));
	if(strcmp(n->header->name, "./") != 0){
		if(strcmp(n->header->typeflag, "5") == 0) {
			//n->header->typeflag[-1] = ' ';
			sprintf(n->header->size, "%011lo", (long int)0);
			//sprintf(n->header->chksum, "%06o", generate_checksum_wew(n->header));
		}

		//printf("%lo\t %lo\n", n->header->mode, n->header);
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

		printf("\tPozitie pentru %s:\t%ld\n", n->header->name, lseek(fdd, 0, SEEK_CUR));
		write(fdd, n->header, sizeof(struct tar_header));

		if(strcmp(n->header->typeflag, "0") == 0){
			int file_size = strtoul(n->header->size, NULL, 8);
			if(file_size!=0) {
				//write(fdd, n->file, 512 * ((file_size/512) + 2));
				write(fdd, n->file, file_size);
				int pos = lseek(fdd, 0, SEEK_CUR);
				lseek(fdd, (512 - pos % 512), SEEK_CUR);
			}
		}
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

	//struct node* parent;
	//parent = get_node_from_path(private_data,"/");
	print_tree2((struct node*)private_data);
	lseek(fdd, 2 * 512, SEEK_CUR);
}

