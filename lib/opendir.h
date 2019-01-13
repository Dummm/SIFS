#ifndef opendir_h
#define opendir_h

#include <fuse.h>

#include "logger.h"

int sifs_opendir(const char*, struct fuse_file_info*);

#endif
