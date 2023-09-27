# Compiler/linker setup ------------------------------------------------------

# Mac OS X-specific flags.  Comment these out if using Linux.
#PLATFORM = osx
#CC       = gcc
#CFLAGS   = -fast -Wall
#OSLIBS   = -Wl,-framework -Wl,IOKit
#LDFLAGS  =

# Linux-specific flags.  Comment these out if using Mac OS X.
PLATFORM = linux
CC       = gcc
CFLAGS   = -O3 -Wall 
OSLIBS   =
LDFLAGS  =

# make all: all OBJS are compiled and executable available
# make <individual>
# make clean
# TODO: find out why -lpthread does not work when put into CFLAGS
# Example programs -----------------------------------------------------------
# lm modifier: for math library

OBJS = imgconv
all: $(OBJS)

imgconv: imgconv.c imgconv_fun.o
	$(CC) $(CFLAGS) -o imgconv imgconv.c imgconv_fun.o -lm

# library
imgconv_fun.o: imgconv_fun.c imgconv_fun.h
	$(CC) $(CFLAGS) -c imgconv_fun.c -lm

# Maintenance and stuff ------------------------------------------------------
clean:
	rm -f $(OBJS) *.o core

