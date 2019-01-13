#include "../lib/destroy.h"

int fd;
int enfd;
char * argument;
void write_tree(struct node *n) {
	if(strcmp(n->header->name, "./") != 0){
		if(strcmp(n->header->typeflag, "5") == 0) {
			sprintf(n->header->size, "%011ld", (long int)0);
		}

		printf("\tPozitie pentru %s:\t%ld\n", n->header->name, lseek(fd, 0, SEEK_CUR));
		write(fd, n->header, sizeof(struct tar_header));

		if(strcmp(n->header->typeflag, "0") == 0){
			int file_size = strtoul(n->header->size, NULL, 8);
			if(file_size!=0) {
				write(fd, n->file, file_size);
				int pos = lseek(fd, 0, SEEK_CUR);
				lseek(fd, (512 - pos % 512), SEEK_CUR);
			}
		}
	}

	int i;
	for (i = 0; i < n->children_size; i++) {
		write_tree(n->children[i]);
	}

}

void sifs_destroy(void* private_data) {
	logger(DEBUG, "[destroy] Started\n");
	off_t ofs = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	int crt = 0;
	while(crt < ofs) {
		crt += write(fd, "\0", ofs);
	}

	lseek(fd, 0, SEEK_SET);
	write_tree((struct node*)private_data);
	logger(DEBUG, "[destroy] Tree written\n");

	lseek(fd, 17 * 512 - 1, SEEK_CUR);
	write(fd, "\0", 1);
	logger(DEBUG, "[destroy] Ended\n");
}

