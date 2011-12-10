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
	
	int bFill = 0;
	if(myFill) {
		bFill = ALL_ON;
	}
	
	for(int x = 0;x < myArrSize; x++) {
		myBits[x] = bFill;
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

bool BitVector::operator [] (int x) const
{
	return get(x);
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

bool BitVector::put(int i, bool b)
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);
	
	bool oup = getRaw(i, myBits);
	
	if(b) {
		setRaw(i, myBits);
	} else {
		resetRaw(i, myBits);
	}
	
	return oup;
}

bool BitVector::set(int i)
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);
	
	bool oup = getRaw(i, myBits);
	
	if(!oup) {
		setRaw(i, myBits);
	}
	
	return oup;
}

bool BitVector::reset(int i)
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);
	
	bool oup = getRaw(i, myBits);
	
	if(oup) {
		resetRaw(i, myBits);
	}
	return oup;
}

void BitVector::flip(int i)
{
	ASSERT(i >= 0);
	ASSERT(i < mySize);
	
	int cell = i/UNIT_SIZE;
	uint32 mask = 1<<(i%UNIT_SIZE);
	
	if((myBits[cell] & mask ) == 0) {
		myBits[cell] |= mask;
	} else {
		myBits[cell] &= ~mask;
	}
}

void BitVector::setRange(int first, int last)
{
	ASSERT(first >= 0);
	ASSERT(first < mySize);
	ASSERT(last >= 0);
	ASSERT(last < mySize);
	ASSERT(first <= last);
	
	//UNIT_SIZE*2 guarantees we have a whole byte
	// you could make it work with just UNIT_SIZE
	// but it's probably faster to fill in small cases in one loop anyway...?
	if(last - first < UNIT_SIZE*2) {
		for(int x=first; x<=last;x++) {
			setRaw(x, myBits);
		}
		return;
	}
	
	int minUnit, maxUnit;
	
	minUnit = first/UNIT_SIZE+((first%UNIT_SIZE==0)?0:1);
	maxUnit= last/UNIT_SIZE-((last%UNIT_SIZE==0)?0:1);
	
	for(int x=first; x < minUnit*UNIT_SIZE;x++) {
		setRaw(x, myBits);
	}
	
	for(int x=maxUnit*UNIT_SIZE+1;x<=last;x++) {
		setRaw(x, myBits);
	}
	
	for(int x=minUnit;x<=maxUnit;x++) {
		myBits[x] = ALL_ON;
	}
}

void BitVector::resetRange(int first, int last)
{
	ASSERT(first >= 0);
	ASSERT(first < mySize);
	ASSERT(last >= 0);
	ASSERT(last < mySize);
	ASSERT(first <= last);
	
	//UNIT_SIZE*2 guarantees we have a whole byte
	// you could make it work with just UNIT_SIZE
	// but it's probably faster to fill in small cases in one loop anyway...?
	if(last - first < UNIT_SIZE*2) {
		for(int x=first; x<=last;x++) {
			resetRaw(x, myBits);
		}
		return;
	}
	
	int minUnit, maxUnit;
	
	minUnit = first/UNIT_SIZE+((first%UNIT_SIZE==0)?0:1);
	maxUnit= last/UNIT_SIZE-((last%UNIT_SIZE==0)?0:1);
	
	for(int x=first; x < minUnit*UNIT_SIZE;x++) {
		resetRaw(x, myBits);
	}
	
	for(int x=maxUnit*UNIT_SIZE+1;x<=last;x++) {
		resetRaw(x, myBits);
	}
	
	for(int x=minUnit;x<=maxUnit;x++) {
		myBits[x] = 0;
	}
}

void BitVector::setAll()
{
	memset(myBits, 0xFF, myArrSize * sizeof(*myBits));
}

void BitVector::resetAll()
{
	memset(myBits, 0x00, myArrSize * sizeof(*myBits));
}

int BitVector::size() const
{
	return mySize;
}

void BitVector::resize(int length)
{
	ASSERT(length>=0);
	
	int oldSize = mySize;
	mySize = length;
	if(mySize <= oldSize) {
		return;
	}
	
	int oldValidCellCount = oldSize/UNIT_SIZE+((oldSize%UNIT_SIZE>0)?1:0);
	int newValidCellCount = length/UNIT_SIZE+((length%UNIT_SIZE>0)?1:0);
	
	if(oldValidCellCount < newValidCellCount) {
		uint32 *temp = myBits;
		myBits = new uint32[newValidCellCount];
		
		for(int x=0;x<oldValidCellCount;x++) {
			myBits[x] = temp[x];
		}
		
		delete temp;
		myArrSize = newValidCellCount;
	}
	
	if(oldSize < mySize) {
		if(myFill) {
			setRange(oldSize,mySize-1);
		} else {
			resetRange(oldSize,mySize-1);
		}
	}
}


