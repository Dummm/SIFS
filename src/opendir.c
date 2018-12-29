#include "../lib/opendir.h"

int sifs_opendir(const char* path, struct fuse_file_info* fi) {
	logger(DEBUG, "[opendir] Started on path: %s\n", path);
	logger(DEBUG, "[opendir] Ended\n");
  return 0;
}
