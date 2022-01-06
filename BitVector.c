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
	ASSERT(sizeof(b->bitStorage.ptr[0]) * 8 == BVUNITBITS);
	ASSERT(BVUNITBYTES * 8 == BVUNITBITS);

	b->size = 0;
	b->arrSize = 0;
	b->fill = FALSE;
	b->usePtr = FALSE;
	BitVector_Resize(b, size);
}

void BitVector_Destroy(BitVector *b)
{
	ASSERT(b != NULL);
	if (b->usePtr) {
		free(b->bitStorage.ptr);
		b->bitStorage.ptr = NULL;
	}
}

void BitVector_Copy(BitVector *dest, const BitVector *src)
{
	int numBytes;
	ASSERT(dest != NULL);
	ASSERT(src != NULL);

	BitVector_Resize(dest, src->size);
	ASSERT(dest->size == src->size);
	dest->fill = src->fill;

	numBytes = dest->arrSize * BVUNITBYTES;
	memcpy(BitVectorGetPtr(dest), BitVectorGetPtr(src), numBytes);
}

void BitVector_Consume(BitVector *dest, BitVector *src)
{
	ASSERT(dest != NULL);
	ASSERT(src != NULL);
	bool oldFill;

	if (dest->usePtr) {
		free(dest->bitStorage.ptr);
	}
	dest->bitStorage = src->bitStorage;
	dest->usePtr = src->usePtr;

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

	oldValidCellCount = (oldSize + BVUNITBITS - 1) / BVUNITBITS;
	newValidCellCount = (size + BVUNITBITS - 1) / BVUNITBITS;

	if (oldValidCellCount < newValidCellCount) {
		int byteLength;
		uint64 tempRaw;
		uint64 *tempPtr;
		bool useOldPtr = b->usePtr;

		if (useOldPtr) {
			tempPtr = b->bitStorage.ptr;
		} else {
			tempRaw = b->bitStorage.raw;
			tempPtr = &tempRaw;
		}

		b->arrSize = newValidCellCount;
		if (b->arrSize > 1) {
			b->bitStorage.ptr = malloc(b->arrSize * BVUNITBYTES);
			b->usePtr = TRUE;
		} else {
			b->usePtr = FALSE;
		}

		byteLength = oldValidCellCount * BVUNITBYTES;
		memcpy(BitVectorGetPtr(b), tempPtr, byteLength);

		if (useOldPtr) {
			free(tempPtr);
		}
	}

	if (oldSize < size) {
		ASSERT(size > 0);
		if (b->fill) {
			BitVector_SetRange(b, oldSize, size - 1);
		} else {
			BitVector_ResetRange(b, oldSize, size - 1);
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


