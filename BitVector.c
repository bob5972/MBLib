/*
 * BitVector.c -- part of MBLib
 *
 * Copyright (c) 2015-2021 Michael Banack <github@banack.net>
 *
 * MIT License
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "BitVector.h"

#include <string.h>
#include <stdlib.h>

#include "MBAssert.h"
#include "MBUtil.h"

void BitVector_CreateWithSize(BitVector *b, int size)
{
	ASSERT(size >= 0);
	ASSERT(b != NULL);
	ASSERT(sizeof(b->bits[0]) * 8 == BVUNITBITS);

	b->size = 0;
	b->arrSize = 1 + (size / BVUNITBITS);
	b->fill = FALSE;
	b->bits = malloc(b->arrSize * sizeof(b->bits[0]));

	b->size = size;

	if (size > 0) {
		BitVector_ResetRange(b, 0, b->size - 1);
	}
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
	if (b->size <= oldSize) {
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

	if (oldValidCellCount < newValidCellCount) {
		int byteLength;
		uint64 *temp = b->bits;
		b->bits = malloc(newValidCellCount * sizeof (b->bits[0]));

		byteLength = oldValidCellCount * sizeof(b->bits[0]);
		memcpy(&b->bits[0], temp, byteLength);

		free(temp);
		b->arrSize = newValidCellCount;
	}

	if (oldSize < size) {
		if (b->fill) {
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


