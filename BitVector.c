/*
 * This file is part of MBLib.
 * Copyright (c) 2015 Michael Banack <bob5972@banack.net>
 *
 * MBLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MBLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MBLib.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * BitVector.c --
 */
#include "BitVector.h"

#include <string.h>
#include <stdlib.h>

#include "mbassert.h"
#include "mbutil.h"

#define BITVECTOR_DEFAULT_SPACE 2

void BitVector_Create(BitVector *b)
{
	ASSERT(b != NULL);
	ASSERT(sizeof(b->bits[0]) * 8 == BVUNITBITS);

	b->size = 0;
	b->arrSize = BITVECTOR_DEFAULT_SPACE;
	b->fill = FALSE;
	b->bits = malloc(b->arrSize * sizeof(b->bits[0]));
}

void BitVector_CreateWithSize(BitVector *b, int size)
{
    //XXX: This might malloc twice.
    BitVector_Create(b);
    BitVector_Resize(b, size);
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


