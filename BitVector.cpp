#include "BitVector.h"

#include <string.h>

#include "mbassert.h"
#include "mbutil.h"

static INLINE bool getRaw(int i, const uint32 *bits)
{
	const int UNIT_SIZE = sizeof(*bits)*8;
	
	return (bits[i/UNIT_SIZE] & (1<<(i%UNIT_SIZE))) != 0;
}

static INLINE void setRaw(int i, uint32 *bits)
{
	const int UNIT_SIZE = sizeof(*bits)*8;
	
	bits[i/UNIT_SIZE] |= (1<<(i%UNIT_SIZE));
}

static INLINE void resetRaw(int i, uint32 *bits)
{
	const int UNIT_SIZE = sizeof(*bits)*8;
	
	bits[i/UNIT_SIZE] &= ~(1<<(i%UNIT_SIZE));
}


BitVector::BitVector()
{
	mySize = 0;
	myArrSize = DEFAULT_SPACE;
	myFill = FALSE;
	myBits = new uint32[myArrSize];
	
	int bFill = 0;
	
	for(int x=0;x<myArrSize;x++) {
		myBits[x] = bFill;
	}
}
	

BitVector::BitVector(int size)
{
	ASSERT(size >= 0);
	
	myFill = FALSE;
	mySize = size;

	myArrSize = (mySize/UNIT_SIZE)+1;
	
	if (myArrSize < DEFAULT_SPACE) {
		myArrSize = DEFAULT_SPACE;
	}
	
	myBits = new uint32[myArrSize];
	
	int bFill = 0;
	
	for(int x=0;x<myArrSize;x++) {
		myBits[x] = bFill;
	}
}

BitVector::BitVector(int size, bool initial)
{
	ASSERT(size >= 0);
	
	myFill = initial;
	mySize = size;
	
	myArrSize = (mySize/UNIT_SIZE)+1;
	
	if (myArrSize < DEFAULT_SPACE) {
		myArrSize = DEFAULT_SPACE;
	}
	
	myBits = new uint32[myArrSize];
	
	if(myFill) {
		setAll();
	} else {
		resetAll();
	}
}

BitVector::BitVector(const BitVector &a)
{
	myFill = a.myFill;
	mySize = a.mySize;
	myArrSize = (mySize/UNIT_SIZE) + 1;
	
	myBits = new uint32[myArrSize];
		
	for(int x = 0;x < myArrSize;x++) {
		myBits[x] = a.myBits[x];
	}
}

BitVector::~BitVector()
{
	delete myBits;
	myBits = NULL;
}

void BitVector::consume(BitVector& a)
{
	delete myBits;
	myBits = a.myBits;
	mySize = a.mySize;
	myArrSize = a.myArrSize;
	
	a.myArrSize = DEFAULT_SPACE;
	a.myBits = new uint32[a.myArrSize];
	a.mySize = 0;
}


void BitVector::makeEmpty()
{
	mySize = 0;
}	

bool BitVector::operator [] (int i) const
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);
	
	return getRaw(i, myBits);
}

bool BitVector::getFillValue() const
{
	return myFill;
}

void BitVector::setFillValue(bool f)
{
	myFill = f;
}

bool BitVector::get(int i) const
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);
	
	return getRaw(i, myBits);
}

void BitVector::put(int i, bool b)
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);
	
	if(b) {
		setRaw(i, myBits);
	} else {
		resetRaw(i, myBits);
	}
}

void BitVector::set(int i)
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);

	setRaw(i, myBits);
}

void BitVector::reset(int i)
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);
	
	resetRaw(i, myBits);
}

void BitVector::flip(int i)
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);
	
	bool value = getRaw(i, myBits);
	
	if (value) {
		resetRaw(i, myBits);
	} else {
		setRaw(i, myBits);
	}
}

void BitVector::setRange(int first, int last)
{
	ASSERT(first >= 0);
	ASSERT(first < mySize);
	ASSERT(last >= 0);
	ASSERT(last < mySize);
	ASSERT(first <= last);
	
	int numBytes;
	int x;
	uint8 *myBytes;
	
	if (last - first + 1 < 8 * 2) {
		for (x = first; x <= last; x++) {
			setRaw(x, myBits);
		}
		return;
	}
	
	x = first;
	while (x % 8 != 0) {
		setRaw(x, myBits);
		x++;
	}
	
	numBytes = (last - x) / 8;
	myBytes = (uint8 *) myBits;
	myBytes += (x/8);
	memset(myBytes, 0xFF, numBytes);
	x+= 8 * numBytes;
	
	while (x <= last) {
		setRaw(x, myBits);
		x++;
	}
}

void BitVector::resetRange(int first, int last)
{
	ASSERT(first >= 0);
	ASSERT(first < mySize);
	ASSERT(last >= 0);
	ASSERT(last < mySize);
	ASSERT(first <= last);
	
	int numBytes;
	int x;
	uint8 *myBytes;
	
	if (last - first + 1 < 8 * 2) {
		for (x = first; x <= last; x++) {
			resetRaw(x, myBits);
		}
		return;
	}
	
	x = first;
	while (x % 8 != 0) {
		resetRaw(x, myBits);
		x++;
	}
	
	numBytes = (last - x) / 8;
	myBytes = (uint8 *) myBits;
	myBytes += (x/8);
	memset(myBytes, 0x00, numBytes);
	x+= 8 * numBytes;
	
	while (x <= last) {
		resetRaw(x, myBits);
		x++;
	}
}

void BitVector::setAll()
{
	uint32 byteLength;
	
	byteLength = (mySize + 7) / 8;
	ASSERT(byteLength / sizeof(*myBits) <= (uint32) myArrSize);
	
	memset(myBits, 0xFF, byteLength);
}

void BitVector::resetAll()
{
	uint32 byteLength;
	
	byteLength = (mySize + 7) / 8;
	ASSERT(byteLength / sizeof(*myBits) <= (uint32) myArrSize);
	
	memset(myBits, 0x00, byteLength);
}

int BitVector::size() const
{
	return mySize;
}

void BitVector::resize(int length)
{
	ASSERT(length >= 0);
	
	int oldSize = mySize;
	mySize = length;
	if(mySize <= oldSize) {
		return;
	}
	
	int oldValidCellCount = oldSize/UNIT_SIZE+((oldSize%UNIT_SIZE>0)?1:0);
	int newValidCellCount = length/UNIT_SIZE+((length%UNIT_SIZE>0)?1:0);
	
	if(oldValidCellCount < newValidCellCount) {
		int byteLength;
		uint32 *temp = myBits;
		myBits = new uint32[newValidCellCount];
		
		byteLength = oldValidCellCount * sizeof(*myBits);
		memcpy(&myBits[0], temp, byteLength);

		delete temp;
		myArrSize = newValidCellCount;
	}
	
	if(oldSize < mySize) {
		if(myFill) {
			setRange(oldSize, mySize-1);
		} else {
			resetRange(oldSize, mySize-1);
		}
	}
}


