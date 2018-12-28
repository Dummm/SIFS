#include "../lib/mkdir.h"

int sifs_mkdir(const char* path, mode_t mode) {
  logger(
    ERROR,
    "UNIMPLEMENTED: mkdir, path: %s, mode: %lo\n",
    path,
    (unsigned long) mode
  );
	logger(DEBUG, "[mkdir] Started on path: %s\n", path);
	logger(DEBUG, "[mkdir] Ended\n");
  return 0;
}
