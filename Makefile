CC=gcc
CXX=g++

CPPFLAGS = -Wall -g 
#CPPFLAGS  = -O3 -fomit-frame-pointer

OBJECTS=mjbassert.o mjbdebug.o MBString.o MBVector.o MBStack.o MBQueue.o MBSet.o BitArray.o IntMap.o

all: MBLib.a

MBLib.a: ${OBJECTS}
	ar cr MBLib.a ${OBJECTS}

mjbdebug.o: mjbdebug.cpp mjbdebug.h

mjbassert.o: mjbassert.cpp mjbassert.h

MBString.o: MBString.cpp MBString.h

MBVector.o: MBVector.cpp MBVector.h

MBStack.o: MBStack.cpp MBStack.h

MBQueue.o: MBQueue.cpp MBQueue.h

MBSet.o: MBSet.cpp MBSet.h

MBListNode.o: MBListNode.cpp MBListNode.h

BitArray.o: BitArray.cpp BitArray.h

IntMap.o: IntMap.cpp IntMap.h

clean:
	rm -f ${OBJECTS} MBLib.a

#test: all
#	./runTests.sh
