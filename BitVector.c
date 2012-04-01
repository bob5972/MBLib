#include "BitVector.h"

#include <string.h>
#include <stdlib.h>

#include "mbassert.h"
#include "mbutil.h"

#define BITVECTOR_DEFAULT_SPACE 2

void BitVector_Create(BitVector *b)
{
	ASSERT(b != NULL);
	b->size = 0;
	b->arrSize = BITVECTOR_DEFAULT_SPACE;
	b->fill = FALSE;
	b->bits = malloc(b->arrSize * sizeof(b->bits[0]));
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
	
	dest->bits = malloc(sizeof(dest->bits[0]) * dest->arrSize);
	
	numBytes = dest->arrSize * sizeof(dest->bits[0]);
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
		uint64 *temp = b->bits;
		b->bits = malloc(newValidCellCount * sizeof (b->bits[0]));
		
		byteLength = oldValidCellCount * sizeof(b->bits[0]);
		memcpy(&b->bits[0], temp, byteLength);

		free(temp);
		b->arrSize = newValidCellCount;
	}
	
	if(oldSize < size) {
		if(b->fill) {
			BitVector_SetRange(b, oldSize, b->size - 1);
		} else {
			BitVector_ResetRange(b, oldSize, b->size - 1);
		}
	}
}

void BitVectorSetRangeGeneric(BitVector *b, int first, int last)
{
	BitVectorWriteRangeGenericImpl(b, first, last, BITVECTOR_WRITE_SET);
}

void BitVectorResetRangeGeneric(BitVector *b, int first, int last)
{
	BitVectorWriteRangeGenericImpl(b, first, last, BITVECTOR_WRITE_RESET);
}

void BitVectorFlipRangeGeneric(BitVector *b, int first, int last)
{
	BitVectorWriteRangeGenericImpl(b, first, last, BITVECTOR_WRITE_FLIP);
}


