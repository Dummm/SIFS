#include "tar_structure.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#define BUFFER_SIZE 1024
 //int check_block_zero(char *buffer){
//	 for(int i=0;i<BUFFER_SIZE;i++){
//		 if(*(char(*)buf)
//		  return 0;

	// return 1;




int parse_header(struct file_tar_header *tar ,const char *argv,unsigned offset ){
	char *buffer=malloc(BUFFER_SIZE);
	//struct file_tar_header *tar=malloc(sizeof(struct file_tar_header));
	int fd=open(argv,O_RDONLY);
	lseek(fd,0,SEEK_SET);
	read(fd,buffer,BUFFER_SIZE);
	//if(check_block_zero(buffer,512)==0){
	//	free(*tar);
		//error handleing
	//	return 0;
	//}


	  for(int i=0;i<BUFFER_SIZE;i++)
	    printf("%c ",buffer[i]);

	return 0;
 }
int main(int argc, char **argv)
{
	//struct tar_archive *archive =NULL;
//	char buffer[512];
	struct file_tar_header *tar=malloc(sizeof(struct file_tar_header));


	//int octalNumber=0;
	parse_header(tar,argv[1],0);

	return 0;
}

