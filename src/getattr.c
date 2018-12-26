#include "../lib/getattr.h"

int _sifs_getattr(const char* path, struct stat* sbuf) {
	return 0;
}

int sifs_getattr(const char* path, struct stat* sbuf) {
  logger(DEBUG, "[getattr] Started\n");
	printf("caca\n");

	int ret;
	ret =	_sifs_getattr(path, sbuf);

  logger(DEBUG, "[getattr] Ended\n");
  return ret;
}

