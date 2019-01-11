#include "../lib/tar_structure.h"

unsigned int generate_checksum(const struct tar_header* h) {
	unsigned int i;
	unsigned char *p = (unsigned char*) h;
	unsigned int res = 256; // ???
	if(strcmp(h->typeflag, "5") == 0) {
		res += 47;
	}
	for (i = 0; i < offsetof(struct tar_header, chksum); i++) {
		res += p[i];
	}
	for (i = offsetof(struct tar_header, typeflag); i < sizeof(struct tar_header); i++) {
		res += p[i];
	}
	return res;
}