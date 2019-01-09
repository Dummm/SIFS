#ifndef destroy_h
#define destroy_h
#include <sys/stat.h> /* mode_t */
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#include "logger.h" /* logger */
#include "tree.h"
void sifs_destroy(void*);
#endif

