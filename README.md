```
  _____ _____ ______ _____ 
 / ____|_   _|  ____/ ____|
| (___   | | | |__ | (___  
 \___ \  | | |  __| \___ \ 
 ____) |_| |_| |    ____) |  V0.1 alpha
|_____/|_____|_|   |_____/   a.k.a (SIFS is a filesystem)
                           
Developed by:
	- Butnaru Ioan-Sorin 			(niros98@gmail.com)
	- Taralesca Ovidiu-Costinel 	(ovidiu.taralesca@gmail.com)
	- Rohozneanu George-Constantin 	(gcrohozneanu@gmail.com)
	Gr. 241, 2nd Year, Faculty of Mathematics3 and Computer Science	                       
	2018-2019, University of Bucharest
	
```	
## Project details
```
	Required fuse version: FUSE 3
	Developed and tested on Linux (Arch/Manjaro distros)

	Build:
		make
	Run:
		./sifs -f [mount_folder-path] [tar-path]
		./sifs -f mountFolder testTar.tar
	FUSE Debug:
		./sifs -f -d [mount_folder-path] [tar-path]
	
	To build a new tar instead of testTar.tar use:
		tar -cvf TAR_NAME.tar ./FILE1 ./FILE2 
							  ^^ note the ./ before files	
		
	Currently working (and tested):
		* Mounting the tar archive to a folder
		* Building a tree from the tar structure using info provided by [sifs_getattr]
		* cd 			[sifs_opendir]
		* ls 			[sifs_readdir]
		* mkdir 		[sifs_mkdir]
		* touch 		[sifs_mknod]
		* Opening/Modifying (nano/vim) FILE [sifs_mknod] [sifs_open] [sifs_read] [sifs_write]
		
		
	Currently working but sometimes causing random bugs, generally hard to recreate:
		* rm FILE 	[sifs_unlink]
		* rm -r FOLDER [sifs_rmdir]
		* Saving changes in the same tar [sifs_destroy]
```
## References
```
	* For debugging purposes, we have used the logger function from: 
		https://github.com/retrohacker/libfuse-template
	
	* Used as implementation example:
		https://github.com/tniessen/memfs-fuse

	* For the checksum function of the tar header:
		https://github.com/rxi/microtar/blob/master/src/microtar.c
		
	* The tar format corresponds to:
		https://www.gnu.org/software/tar/manual/html_node/Standard.html
```
