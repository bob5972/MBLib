# -*- mode: Makefile;-*-
# Makefile --
#    Q: Can anything be sadder than work left unfinished?
#    A: Yes, work never begun.

INCLUDEDIR=public
SRCROOT=.
BUILDROOT=build
TMPDIR=$(BUILDROOT)/tmp
DEPROOT=$(BUILDROOT)/deps

include $(BUILDROOT)/config.mk

OPTIMIZATION_FLAGS = -Wall -g 
#OPTIMIZATION_FLAGS = -O3 -fomit-frame-pointer
#OPTIMIZATION_FLAGS = -O3 -ftree-vectorizer-verbose=2

CFLAGS = -I . -I ${INCLUDEDIR} -I $(BUILDROOT) ${OPTIMIZATION_FLAGS}

CPPFLAGS= ${CFLAGS}

CC=gcc
CXX=g++

$(BUILDROOT)/%.opp: $(SRCROOT)/%.cpp
	${CXX} -c ${CPPFLAGS} -o $(BUILDROOT)/$*.opp $<;
	
#Autogenerate dependencies information
#The generated makefiles get source into this file later
$(DEPROOT)/%.dpp: $(SRCROOT)/%.cpp Makefile
	$(CXX) -M -MM -MT "$(BUILDROOT)/$*.opp" -MT "$(DEPROOT)/$*.dpp" \
	    -MF $@ ${CPPFLAGS} $<;
	    
SOURCES = mjbdebug.cpp \
          mjbassert.cpp \
          MBString.cpp \
          MBVector.cpp \
          MBStack.cpp \
          MBQueue.cpp \
          MBSet.cpp \
          BitArray.cpp \
          IntMap.cpp \
          MBMap.cpp \
          MBMatrix.cpp \
          IntSet.cpp
          
OBJECTS=$(addprefix $(BUILDROOT)/, \
            $(subst .cpp,.opp, $(SOURCES)))

#The config check is to test if we've been configured
all: $(BUILDROOT)/config.h MBLib.a

.PHONY: all clean distclean

test: test.bin
	./test.bin

test.bin: MBLib.a test.cpp
	g++ ${CPPFLAGS} -g test.cpp MBLib.a -o test.bin

MBLib.a: ${OBJECTS}
	ar cr MBLib.a ${OBJECTS}

clean:
	rm -f MBLib.a test.bin
	rm -f $(BUILDROOT)/*.o $(BUILDROOT)/*.opp

distclean:
	rm -rf $(BUILDROOT)/

#include the generated dependency files
-include $(addprefix $(DEPROOT)/,$(subst .cpp,.dpp,$(SOURCES)))

