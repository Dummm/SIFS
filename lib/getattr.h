#ifndef getattr_h
#define getattr_h
#include "../lib/logger.h"
#include <sys/stat.h>
int sifs_getattr(const char*, struct stat*);
#endif
