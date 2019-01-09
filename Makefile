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
CFLAGS  = -g -Wall `pkg-config fuse3 --cflags --libs` -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=31

# typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: sifs

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#
sifs: \
	sifs.o \
	src/logger.o \
	src/tree.o \
	src/getattr.o \
	src/readdir.o \
	src/opendir.o \
	src/open.o \
	src/read.o \
	src/mknod.o \
	src/mkdir.o \
	src/destroy.o \
	src/write.o 
	$(CC) $(CFLAGS) -o \
	sifs \
	src/logger.o \
	src/tree.o \
	src/getattr.o \
	src/readdir.o \
	src/opendir.o \
	src/mkdir.o \
	src/mknod.o \
	src/open.o \
	src/read.o \
	src/write.o \
	src/destroy.o \
	sifs.o


# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#
write.o:  src/write.c lib/write.h
	$(CC) $(CFLAGS) -c write.c

logger.o:  src/logger.c lib/logger.h
	$(CC) $(CFLAGS) -c logger.c

tree.o:  src/tree.c lib/tree.h
	$(CC) $(CFLAGS) -c tree.c


mkdir.o: src/mkdir.c lib/mkdir.h
	$(CC) $(CFLAGS) -c mkdir.c
	
getattr.o: src/getattr.c lib/getattr.h
	$(CC) $(CFLAGS) -c getattr.c

readdir.o: src/readdir.c lib/readdir.h
	$(CC) $(CFLAGS) -c readdir.c

opendir.o: src/opendir.c lib/opendir.h
	$(CC) $(CFLAGS) -c opendir.c

open.o: src/open.c lib/open.h
	$(CC) $(CFLAGS) -c open.c

read.o: src/read.c lib/read.h
	$(CC) $(CFLAGS) -c read.c

mknod.o: src/mknod.c lib/mknod.h
	$(CC) $(CFLAGS) -c mknod.c
	
destroy.o: src/destroy.c lib/destroy.h
	$(CC) $(CFLAGS) -c destroy.c

sifs.o: sifs.c lib/tar_structure.h
	$(CC) $(CFLAGS) -c sifs.c

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean:
	$(RM) sifs *.o *~
