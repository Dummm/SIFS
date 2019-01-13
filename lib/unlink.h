#ifndef unlink_h
#define unlink_h
#include "./logger.h" /* logger */
#include <fuse.h> /* fuse_file_info */
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fuse.h>
#include "tree.h"
#include "logger.h"
#include "tar_structure.h"
int sifs_unlink(const char* path);
#endif

