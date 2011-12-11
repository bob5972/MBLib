#include "BitVector.h"

#include <string.h>
#include <stdlib.h>

#include "mbassert.h"
#include "mbutil.h"

#define BITVECTOR_DEFAULT_SPACE 1

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

void BitVector_Create(BitVector *b)
{
	ASSERT(b != NULL);
	b->size = 0;
	b->arrSize = BITVECTOR_DEFAULT_SPACE;
	b->fill = FALSE;
	b->bits = malloc(b->arrSize * sizeof(*b->bits));
}

void BitVector_Resize(BitVector *b, int size)
{
	const int UNIT_SIZE = sizeof(*b->bits)*8;
	int oldSize;
	int oldValidCellCount;
	int newValidCellCount;
	
	ASSERT(b != NULL);
	ASSERT(size >= 0);
	
	oldSize = b->size;
	b->size = size;
	if(b->size <= oldSize) {
		return;
	}
	
	oldValidCellCount = oldSize / UNIT_SIZE;
	if (oldSize % UNIT_SIZE > 0) {
		oldValidCellCount++;
	}
	
	newValidCellCount = size / UNIT_SIZE;
	if (size % UNIT_SIZE > 0) {
		newValidCellCount++;
	}
	
	if(oldValidCellCount < newValidCellCount) {
		int byteLength;
		uint32 *temp = b->bits;
		b->bits = malloc(newValidCellCount * sizeof (*b->bits));
		
		byteLength = oldValidCellCount * sizeof(*b->bits);
		memcpy(&b->bits[0], temp, byteLength);

		free(temp);
		b->arrSize = newValidCellCount;
	}
	
	if(oldSize < b->size) {
		if(b->fill) {
			BitVector_SetRange(b, oldSize, b->size-1);
		} else {
			BitVector_ResetRange(b, oldSize, b->size-1);
		}
	}
}


bool BitVector_Get(const BitVector *b, int x)
{
	ASSERT(b != NULL);
	ASSERT(x >= 0);
	ASSERT(x < b->size);
	
	return getRaw(x, b->bits);
}


void BitVector_Put(BitVector *b, int i, bool v)
{
	ASSERT(b != NULL);
	ASSERT(i >= 0);
	ASSERT(i < b->size);
	
	if(v) {
		setRaw(i, b->bits);
	} else {
		resetRaw(i, b->bits);
	}
}

void BitVector_Set(BitVector *b, int i)
{
	ASSERT(b != NULL);
	ASSERT(i >= 0);
	ASSERT(i < b->size);

	setRaw(i, b->bits);
}

void BitVector_Reset(BitVector *b, int i)
{
	ASSERT(b != NULL);
	ASSERT(i >= 0);
	ASSERT(i < b->size);
	
	resetRaw(i, b->bits);
}


void BitVector_Flip(BitVector *b, int i)
{
	ASSERT(b != NULL);
	ASSERT(i >= 0);
	ASSERT(i < b->size);
	
	bool value = getRaw(i, b->bits);
	
	if (value) {
		resetRaw(i, b->bits);
	} else {
		setRaw(i, b->bits);
	}
}


void BitVector_SetRange(BitVector *b, int first, int last)
{
	ASSERT(b != NULL);
	ASSERT(first >= 0);
	ASSERT(first < b->size);
	ASSERT(last >= 0);
	ASSERT(last < b->size);
	ASSERT(first <= last);
	
	int numBytes;
	int x;
	uint8 *myBytes;
	
	if (last - first + 1 < 8 * 2) {
		for (x = first; x <= last; x++) {
			setRaw(x, b->bits);
		}
		return;
	}
	
	x = first;
	while (x % 8 != 0) {
		setRaw(x, b->bits);
		x++;
	}
	
	numBytes = (last - x) / 8;
	myBytes = (uint8 *) b->bits;
	myBytes += (x/8);
	memset(myBytes, 0xFF, numBytes);
	x+= 8 * numBytes;
	
	while (x <= last) {
		setRaw(x, b->bits);
		x++;
	}
}

void BitVector_ResetRange(BitVector *b, int first, int last)
{
	ASSERT(b != NULL);
	ASSERT(first >= 0);
	ASSERT(first < b->size);
	ASSERT(last >= 0);
	ASSERT(last < b->size);
	ASSERT(first <= last);
	
	int numBytes;
	int x;
	uint8 *myBytes;
	
	if (last - first + 1 < 8 * 2) {
		for (x = first; x <= last; x++) {
			resetRaw(x, b->bits);
		}
		return;
	}
	
	x = first;
	while (x % 8 != 0) {
		resetRaw(x, b->bits);
		x++;
	}
	
	numBytes = (last - x) / 8;
	myBytes = (uint8 *) b->bits;
	myBytes += (x/8);
	memset(myBytes, 0x00, numBytes);
	x+= 8 * numBytes;
	
	while (x <= last) {
		resetRaw(x, b->bits);
		x++;
	}
}

void BitVector_SetAll(BitVector *b)
{
	ASSERT(b != NULL);
	uint32 byteLength;
	
	byteLength = (b->size + 7) / 8;
	ASSERT(byteLength / sizeof(*b->bits) <= (uint32) b->arrSize);
	
	memset(b->bits, 0xFF, byteLength);
}

void BitVector_ResetAll(BitVector *b)
{
	ASSERT(b != NULL);
	uint32 byteLength;
	
	byteLength = (b->size + 7) / 8;
	ASSERT(byteLength / sizeof(*b->bits) <= (uint32) b->arrSize);
	
	memset(b->bits, 0x00, byteLength);
}

bool BitVector_GetFillValue(const BitVector *b)
{
	ASSERT(b != NULL);
	return b->fill;
}

void BitVector_SetFillValue(BitVector *b, bool f)
{
	ASSERT(b != NULL);
	b->fill = f;
}

void BitVector_Copy(BitVector *dest, const BitVector *src)
{
	const int UNIT_SIZE = sizeof(*dest->bits)*8;
	int numBytes;
	ASSERT(dest != NULL);
	ASSERT(src != NULL);
	
	dest->fill = src->fill;
	dest->size = src->size;
	dest->arrSize = (dest->size/UNIT_SIZE) + 1;
	
	dest->bits = malloc(sizeof(*dest->bits) * dest->arrSize);
	
	numBytes = dest->arrSize * sizeof(*dest->bits);
	memcpy(dest->bits, src->bits, numBytes);
}

void BitVector_Destroy(BitVector *b)
{
	ASSERT(b != NULL);
	free(b->bits);
	b->bits = NULL;
}

void BitVector_Consume(BitVector *dest, BitVector *src)
{
	ASSERT(dest != NULL);
	ASSERT(src != NULL);
	bool oldFill;
	
	free(dest->bits);
	dest->bits = src->bits;
	dest->size = src->size;
	dest->arrSize = src->arrSize;
	
	oldFill = src->fill;
	BitVector_Create(src);
	src->fill = oldFill;
}


