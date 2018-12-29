#ifndef read_h
#define read_h
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
int sifs_read(const char*, char*, size_t, off_t, struct fuse_file_info*);
#endif

