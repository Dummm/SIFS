#ifndef mkdir_h
#define mkdir_h

#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#include <pwd.h>
#include <grp.h>

#include "logger.h"
#include "tree.h"
#include "fuse.h"

int sifs_mkdir(const char*, mode_t);

#endif
