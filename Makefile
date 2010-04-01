CC=gcc
CXX=g++

CPPFLAGS = -Wall -g 
#CPPFLAGS  = -O3 -fomit-frame-pointer
#CPPFLAGS = -O3 -ftree-vectorizer-verbose=2

OBJECTS=mjbassert.o mjbdebug.o MBString.o MBVector.o MBStack.o \
		MBQueue.o MBSet.o BitArray.o IntMap.o MBMap.o  MBMatrix.o \
		IntSet.o

all: MBLib.a

test: test.bin
	./test.bin

test.bin: MBLib.a test.cpp
	g++ -g test.cpp MBLib.a -o test.bin

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
MBMap.o: MBMap.cpp MBMap.h
MBMatrix.o: MBMatrix.cpp MBMatrix.h
IntSet.o: IntSet.cpp IntSet.h

clean:
	rm -f ${OBJECTS} MBLib.a test.bin

#test: all
#	./runTests.sh
