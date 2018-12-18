

/* Refrence wiki tar(computing);
 * https://en.wikipedia.org/wiki/Tar_(computing)
 *
 *
 *
 * https://www.gnu.org/software/tar/manual/html_node/Standard.html
 */

struct tar_archive{
	char archive_name[50];
	struct file_tar_header **headers;
	int number_of_files_in_tar;
};

/* Descriptor for a single file hole.  */
struct sparse {					// byte offset
  char offset[12];  		//   0
  char numbytes[12];		//  12
};

#define SPARSES_IN_STAR_HEADER      4
#define SPARSES_IN_STAR_EXT_HEADER  21

struct file_tar_header {// byte offset
	char name[100];				//   0
	char mode[8];					// 100
	char uid[8];					// 108
	char gid[8];					// 116
	char size[12];				// 124
	char mtime[12];				// 136
	char chksum[8];				// 148
	char typeflag[1];			// 156
	char linkname[100];		// 157

	/// UStar
	char magic[6];				// 257
	char version[2];			// 263
	char uname[32];				// 265
	char gname[32];				// 297
	char devmajor[8];			// 329
	char devminor[8];			// 337
	char prefix[1];			// 345
  char fill2[1];           // 346
  char fill3[8];        // 347
  char isextended[1];      // 355
  struct sparse sp[SPARSES_IN_STAR_HEADER]; // 356
  char realsize[12];    // 452  Actual size of the file
  char offset[12];      // 464  Offset of multivolume contents
  char atime[12];       // 476
  char ctime[12];       // 488
  char mfill[8];        // 500
  char xmagic[4];       // 508  "tar"
};
