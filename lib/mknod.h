#include <sys/stat.h> /* mode_t */
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#include "logger.h" /* logger */
#include "tree.h"
#include "fuse.h"
int sifs_mknod(const char*, mode_t);

