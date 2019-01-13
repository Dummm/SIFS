#ifndef unlink_h
#define unlink_h

#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fuse.h>

#include "tree.h"
#include "logger.h"

int sifs_unlink(const char* path);

#endif

