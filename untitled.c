#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tar_structure.h"

#define BUFFER_SIZE 10240

int parse_header(struct file_tar_header *tar, const char *argv, unsigned offset) {
	char *buffer = malloc(BUFFER_SIZE);

	struct file_tar_header *auxTar = malloc(sizeof(struct file_tar_header));

	int fd = open(argv, O_RDONLY);
	lseek(fd, 0, SEEK_SET);
	//read(fd, buffer, BUFFER_SIZE);
	read(fd, auxTar, sizeof(struct file_tar_header));

	printf("%s ", auxTar);
	/*
	for(int i = 0; i < BUFFER_SIZE; i++)
		printf("%c ", buffer[i]);
	*/
	return 0;
}

int main(int argc, char **argv) {
	struct file_tar_header *auxTar = malloc(sizeof(struct file_tar_header));

	int fd;
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		printf("File open error: %d\n", errno);

	lseek(fd, 0, SEEK_SET);

	while(read(fd, auxTar, sizeof(struct file_tar_header))) {
		if(auxTar->name[0] == '\0') break;

		printf("%d\n", lseek(fd, 0, SEEK_CUR));

		printf("%s\n", auxTar->name);
		printf("\tFile mode: %s\n", auxTar->mode);
		printf("\tOwner ID: %s\n", auxTar->uid);
		printf("\tGroup ID: %s\n", auxTar->gid);
		printf("\tSize: %s\n", auxTar->size);
		printf("\tModification time: %s\n", auxTar->mtime);
		printf("\tChecksum: %s\n", auxTar->chksum);
		printf("\tType flag: %s\n", auxTar->typeflag);
		printf("\tLink name: %s\n", auxTar->linkname);

		/// UStar
		printf("\tUStar: %s\n", auxTar->magic);
		printf("\tUStar version: %s\n", auxTar->version);
		printf("\tOwner name: %s\n", auxTar->uname);
		printf("\tGroup name: %s\n", auxTar->gname);
		printf("\tDevice major: %s\n", auxTar->devmajor);
		printf("\tDevice minor: %s\n", auxTar->devminor);
		printf("\tPrefix: %s\n", auxTar->prefix);
		//printf("\t: %s\n", auxTar->fill2);
		//printf("\t: %s\n", auxTar->fill3);
		printf("\tIs extended: %s\n", auxTar->isextended);
		//printf("\t: %s\n", auxTar->sparse);
		printf("\tReal size: %s\n", auxTar->realsize);
		printf("\tOffset: %s\n", auxTar->offset);
		printf("\tAccess time: %s\n", auxTar->atime);
		printf("\tCreation Time: %s\n", auxTar->ctime);
		//printf("\t: %s\n", auxTar->mfill);
		printf("\tX magic: %s\n", auxTar->xmagic);

		if(strcmp(auxTar->typeflag, "5") != 0) {
			int sz = strtoul(auxTar->size, NULL, 8);
			char *fileContent = malloc(sz);
			read(fd, fileContent, sz);
			printf("File content[%d]:\n", sz);
			printf("%s", fileContent);
			free(fileContent);

			int pos = lseek(fd, 0, SEEK_CUR);
			//printf("%d", 512 - (pos % 512));
			lseek(fd, 512 - (pos % 512), SEEK_CUR);
		}

		printf("\n");
	}


	printf("%d\n\n", lseek(fd, 0, SEEK_END));


	free(auxTar);
	close(fd);
	return 0;
}

