#ifndef readdir_h
#define readdir_h

#include <sys/stat.h>
#include <stdlib.h>
#include <fuse.h>

#include "tree.h"
#include "logger.h"

int sifs_readdir(
    const char*,
    void*,
    fuse_fill_dir_t,
    off_t,
    struct fuse_file_info*,
		enum fuse_readdir_flags
);

#endif
