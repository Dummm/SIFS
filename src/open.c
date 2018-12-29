#include "../lib/open.h"

int sifs_open(const char* path, struct fuse_file_info* fi) {
  logger(ERROR, "[open] File opened, path: %s\n", path);
  return 0;
}

