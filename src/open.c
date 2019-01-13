#include "../lib/open.h"

int sifs_open(const char* path, struct fuse_file_info* fi) {
  logger(ERROR, "[open] Started on path: %s\n", path);
  logger(ERROR, "[open] Ended");
  return 0;
}

