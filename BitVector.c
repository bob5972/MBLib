#include "BitVector.h"

#include <string.h>
#include <stdlib.h>

#include "mbassert.h"
#include "mbutil.h"

#define BITVECTOR_DEFAULT_SPACE 1

void BitVector_Create(BitVector *b)
{
	ASSERT(b != NULL);
	b->size = 0;
	b->arrSize = BITVECTOR_DEFAULT_SPACE;
	b->fill = FALSE;
	b->bits = malloc(b->arrSize * sizeof(*b->bits));
}

void BitVector_Destroy(BitVector *b)
{
	ASSERT(b != NULL);
	free(b->bits);
	b->bits = NULL;
}

void BitVector_Copy(BitVector *dest, const BitVector *src)
{
	int numBytes;
	ASSERT(dest != NULL);
	ASSERT(src != NULL);
	
	dest->fill = src->fill;
	dest->size = src->size;
	dest->arrSize = (dest->size/BVUNITBITS) + 1;
	
	dest->bits = malloc(sizeof(*dest->bits) * dest->arrSize);
	
	numBytes = dest->arrSize * sizeof(*dest->bits);
	memcpy(dest->bits, src->bits, numBytes);
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

void BitVector_Resize(BitVector *b, int size)
{
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
	
	/*
	 * Account for stray bits, and leave an extra cell at the end
	 * even if there aren't any (makes PopCount easier).
	 */
	oldValidCellCount = oldSize / BVUNITBITS;
	oldValidCellCount++;
	
	newValidCellCount = size / BVUNITBITS;
	newValidCellCount++;
	
	if(oldValidCellCount < newValidCellCount) {
		int byteLength;
		uint32 *temp = b->bits;
		b->bits = malloc(newValidCellCount * sizeof (*b->bits));
		
		byteLength = oldValidCellCount * sizeof(*b->bits);
		memcpy(&b->bits[0], temp, byteLength);

		free(temp);
		b->arrSize = newValidCellCount;
	}
	
	if(oldSize < size) {
		if(b->fill) {
			BitVector_SetRange(b, oldSize, b->size-1);
		} else {
			BitVector_ResetRange(b, oldSize, b->size-1);
		}
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
			BitVector_SetRaw(x, b->bits);
		}
		return;
	}
	
	x = first;
	while (x % 8 != 0) {
		BitVector_SetRaw(x, b->bits);
		x++;
	}
	
	numBytes = (last - x) / 8;
	myBytes = (uint8 *) b->bits;
	myBytes += (x/8);
	memset(myBytes, 0xFF, numBytes);
	x += 8 * numBytes;
	
	while (x <= last) {
		BitVector_SetRaw(x, b->bits);
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
			BitVector_ResetRaw(x, b->bits);
		}
		return;
	}
	
	x = first;
	while (x % 8 != 0) {
		BitVector_ResetRaw(x, b->bits);
		x++;
	}
	
	numBytes = (last - x) / 8;
	myBytes = (uint8 *) b->bits;
	myBytes += (x/8);
	memset(myBytes, 0x00, numBytes);
	x += 8 * numBytes;
	
	while (x <= last) {
		BitVector_ResetRaw(x, b->bits);
		x++;
	}
}

uint BitVector_PopCount(const BitVector *b)
{
	int x;
	int size;
	int cellSize;
	int sum;
	
	int strayBitCount;
	uint32 strayBitMask;
	
	ASSERT(b != NULL);
	
	sum = 0;
	size = b->size;
	cellSize = size / BVUNITBITS;
	
	for (x = 0; x < cellSize; x++) {
		sum += popcount(b->bits[x]);
	}
	
	strayBitCount = size % BVUNITBITS;	
	strayBitMask = (1 << strayBitCount) - 1;
		
	ASSERT(cellSize < b->arrSize);
	sum += popcount(b->bits[cellSize] & strayBitMask);
	
	return sum;
}

