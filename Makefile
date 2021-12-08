# -*- mode: Makefile;-*-
# Makefile -- MBLib
#    Q: Can anything be sadder than work left unfinished?
#    A: Yes, work never begun.

include config.mk
-include local.mk

INCLUDE_FLAGS=-I $(MBLIB_SRCDIR)/ -I $(MBLIB_SRCDIR)/public

#The BUILDROOT folder is included for config.h
CFLAGS = --std=gnu11 ${DEFAULT_CFLAGS} ${INCLUDE_FLAGS} -I $(BUILDROOT)
CPPFLAGS = ${DEFAULT_CFLAGS} ${INCLUDE_FLAGS} -I $(BUILDROOT)

LIBFLAGS =
ifeq ($(MB_HAS_SDL2), 1)
	LIBFLAGS += -lSDL2
endif

$(MBLIB_BUILDDIR)/%.opp: $(MBLIB_SRCDIR)/%.cpp
	${CXX} -c ${CPPFLAGS} -o $(MBLIB_BUILDDIR)/$*.opp $<;

$(MBLIB_BUILDDIR)/%.o: $(MBLIB_SRCDIR)/%.c
	${CC} -c ${CFLAGS} -o $(MBLIB_BUILDDIR)/$*.o $<;


#Autogenerate dependencies information
#The generated makefiles get source into this file later
ifeq ($(CC), gcc)
$(DEPROOT)/%.dpp: $(MBLIB_SRCDIR)/%.cpp Makefile
	$(CXX) -M -MP -MM -MT "$(MBLIB_BUILDDIR)/$*.opp" \
	    -MT "$(MBLIB_DEPDIR)/$*.dpp" \
	    -MF $@ ${CPPFLAGS} $<;
$(DEPROOT)/%.d: $(MBLIB_SRCDIR)/%.c Makefile
	$(CC) -M -MP -MM -MT "$(MBLIB_BUILDDIR)/$*.o" \
	    -MT "$(MBLIB_DEPDIR)/$*.d" \
	    -MF $@ ${CPPFLAGS} $<;
endif

CPP_SOURCES = MBString.cpp \
              MBVector.cpp \
              MBStack.cpp \
              MBQueue.cpp \
              MBSet.cpp \
              MBMatrix.cpp \
              MBSocket.cpp \
              Dumper.cpp

C_SOURCES = BitVector.c \
	    MBVarMap.c \
            MBAssert.c \
            MBDebug.c \
            MBOpt.c \
            MBRegistry.c \
            MBString.c \
            MBVector.c \
            MBRing.c \
            MBCompare.c \
            Random.c

OBJECTS=$(addprefix $(MBLIB_BUILDDIR)/, \
          $(subst .cpp,.opp, $(CPP_SOURCES))) \
        $(addprefix $(MBLIB_BUILDDIR)/, \
          $(subst .c,.o, $(C_SOURCES)))

#The config check is to test if we've been configured
all: $(BUILDROOT)/config.h $(MBLIB_BUILDDIR)/MBLib.a testbin benchmarkbin

.PHONY: all clean distclean dist

testbin: $(MBLIB_BUILDDIR)/test.bin

test: testbin
	$(MBLIB_BUILDDIR)/test.bin

benchmarkbin: $(MBLIB_BUILDDIR)/benchmark.bin

$(MBLIB_BUILDDIR)/test.bin: $(MBLIB_BUILDDIR)/MBLib.a $(MBLIB_SRCDIR)/public/* $(MBLIB_SRCDIR)/test.cpp
	${CXX} ${CPPFLAGS} -g $(MBLIB_SRCDIR)/test.cpp \
					   $(MBLIB_BUILDDIR)/MBLib.a \
					   $(LIBFLAGS) \
					   -o $(MBLIB_BUILDDIR)/test.bin

$(MBLIB_BUILDDIR)/benchmark.bin: $(MBLIB_BUILDDIR)/MBLib.a $(MBLIB_SRCDIR)/public/* $(MBLIB_SRCDIR)/test.cpp
	${CXX} -D BENCHMARK ${CPPFLAGS} $(MBLIB_SRCDIR)/test.cpp \
									$(MBLIB_BUILDDIR)/MBLib.a \
									$(LIBFLAGS) \
									-o $(MBLIB_BUILDDIR)/benchmark.bin

$(MBLIB_BUILDDIR)/MBLib.a: ${OBJECTS}
	ar cr $(MBLIB_BUILDDIR)/MBLib.a ${OBJECTS}

clean:
	rm -f $(MBLIB_BUILDDIR)/MBLib.a
	rm -f $(MBLIB_BUILDDIR)/test.bin
	rm -f $(MBLIB_BUILDDIR)/benchmark.bin
	rm -f $(MBLIB_BUILDDIR)/*.o $(MBLIB_BUILDDIR)/*.opp

dist:
	rm -f $(MBLIB_BUILDDIR)/MBLib.tar.bz2
	rm -rf $(TMPDIR)/MBLib
	mkdir $(TMPDIR)/MBLib
	cp -r $(MBLIB_SRCDIR)/*.c $(MBLIB_SRCDIR)/*.cpp Makefile configure public \
	      $(TMPDIR)/MBLib
	cd $(TMPDIR); tar -cjf MBLib.tar.bz2 MBLib
	mv $(TMPDIR)/MBLib.tar.bz2 $(MBLIB_BUILDDIR)/MBLib.tar.bz2

distclean:
	rm config.mk
	rm -rf $(MBLIB_DEPDIR)/
	rm -rf $(MBLIB_BUILDDIR)/

#include the generated dependency files
ifeq ($(CC), gcc)
-include $(addprefix $(MBLIB_DEPDIR)/,$(subst .cpp,.dpp,$(CPP_SOURCES)))
-include $(addprefix $(MBLIB_DEPDIR)/,$(subst .c,.d,$(C_SOURCES)))
endif
