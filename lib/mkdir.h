#include <sys/stat.h> /* mode_t */
#include <string.h>
#include <stdlib.h>
#include "logger.h" /* logger */
#include "tree.h"
#include "fuse.h"
int sifs_mkdir(const char*, mode_t);
