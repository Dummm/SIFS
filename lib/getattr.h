#ifndef getattr_h
#define getattr_h

#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fuse.h>
#include "tree.h"
#include "logger.h"
int sifs_getattr(const char*, struct stat*, struct fuse_file_info*);

#endif
