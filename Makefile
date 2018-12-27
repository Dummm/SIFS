#
# This is an example Makefile for a countwords program.  This
# program uses both the scanner module and a counter module.
# Typing 'make' or 'make count' will create the executable file.
#

# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = gcc
CFLAGS  = -g -Wall `pkg-config fuse --cflags --libs` -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=31

# typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: sifs

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#

sifs:  sifs.o src/logger.o src/tree.o src/getattr.o
	$(CC) $(CFLAGS) -o sifs src/logger.o src/tree.o src/getattr.o sifs.o

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#
logger.o:  src/logger.c lib/logger.h
	$(CC) $(CFLAGS) -c logger.c

# To create the object file counter.o, we need the source files
# counter.c and counter.h:
#
tree.o:  src/tree.c lib/tree.h
	$(CC) $(CFLAGS) -c tree.c

getattr.o: src/getattr.c lib/getattr.h
	$(CC) $(CFLAGS) -c getattr.c

sifs.o: sifs.c lib/tar_structure.h
	$(CC) $(CFLAGS) -c sifs.c


# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean:
	$(RM) sifs *.o *~
