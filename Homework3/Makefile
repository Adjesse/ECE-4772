# Compiler/linker setup ------------------------------------------------------

# Mac OS X-specific flags.  Comment these out if using Linux.
#PLATFORM = osx
#CC       = gcc
#CFLAGS   = -fast -Wall
#OSLIBS   = -Wl,-framework -Wl,IOKit
#LDFLAGS  =

# Linux-specific flags.  Comment these out if using Mac OS X.
PLATFORM = linux
CC       = g++
CFLAGS   = -O3 -Wall -std=c++11
OSLIBS   =
LDFLAGS  = -ltbb

#-std=c++11: special modifier to allow for lambda expressions in TBB

# make all: all OBJS are compiled and executable available
# make <individual>
# make clean
# '-ltbb' has to be included at the end of the command (can't be in the CFLAGS)
# do not use spaces (tabs are allowed)
# Example programs -----------------------------------------------------------

OBJS = morpho
all: $(OBJS)

morpho: morpho.cpp morpho_fun.o
	$(CC) $(CFLAGS) morpho.cpp morpho_fun.o $(LDFLAGS) -o morpho

# library
morpho_fun.o: morpho_fun.cpp morpho_fun.h
	$(CC) $(CFLAGS) -c morpho_fun.cpp $(LDFLAGS)

# Maintenance and stuff ------------------------------------------------------
clean:
	rm -f $(OBJS) *.o core