#ifndef open_h
#define open_h

#include <unistd.h>
#include <fuse.h>

#include "logger.h"

int sifs_open(const char*, struct fuse_file_info*);

#endif

