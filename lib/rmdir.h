#ifndef rmdir_h
#define rmdir_h

#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fuse.h>

#include "tree.h"
#include "logger.h"
#include "tar_structure.h"

int sifs_rmdir(const char* path);

#endif

