/*
 * BitVector.h -- part of MBLib
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

#ifndef BitVector_H_201001192326
#define BitVector_H_201001192326

#ifdef __cplusplus
    extern "C" {
#endif

#include "MBTypes.h"
#include "MBUtil.h"
#include "MBAssert.h"
#include <string.h>

typedef struct BitVector {
    uint64 *bits;
    uint arrSize;
    uint size;
    bool fill;
} BitVector;

typedef BitVector CBitVector;

typedef enum BitVectorWriteType {
    BITVECTOR_WRITE_RESET,
    BITVECTOR_WRITE_SET,
    BITVECTOR_WRITE_FLIP,
} BitVectorWriteType;

void BitVector_CreateWithSize(BitVector *b, int size);
static inline void BitVector_Create(BitVector *b)
{
    BitVector_CreateWithSize(b, 0);
}

void BitVector_Destroy(BitVector *b);
void BitVector_Copy(BitVector *dest, const BitVector *src);

//This is a weird function.
//It empties dest, copies over everything from src,
//  and then leaves src empty.
//Fill is left unchanged in both.
void BitVector_Consume(BitVector *dest, BitVector *src);

void BitVector_Resize(BitVector *b, int size);

// Helper function for fills
void BitVectorSetRangeGeneric(BitVector *b, int first, int last);
void BitVectorResetRangeGeneric(BitVector *b, int first, int last);
void BitVectorFlipRangeGeneric(BitVector *b, int first, int last);

static inline uint64 *BitVectorGetPtr(const BitVector *b) {
    BitVector *bv = (BitVector *)b;
    return bv->bits;
}

#define BVUNITBITS 64
#define BVUNITBYTES (BVUNITBITS / 8)
#define BVINDEX(x) (x / BVUNITBITS)
#define BVMASK(x)  ( ((uint64)1) << (x % BVUNITBITS))

static inline bool BitVector_GetRaw16(int i, uint16 bits)
{
    ASSERT(i >= 0);
    ASSERT(i < 16);
    return (bits & BVMASK(i)) != 0;
}


static inline bool BitVector_GetRaw32(int i, uint32 bits)
{
    ASSERT(i >= 0);
    ASSERT(i < 32);
    return (bits & BVMASK(i)) != 0;
}

static inline bool BitVector_GetRaw64(int i, uint64 bits)
{
    ASSERT(i >= 0);
    ASSERT(i < 64);
    return (bits & BVMASK(i)) != 0;
}

static inline bool BitVector_GetRaw(int i, const uint64 *bits)
{
#if defined(__GNUC__) && (defined(ARCH_AMD64) || defined(ARCH_x86))
    if (!CONSTANT(i)) {
        uint32 tmp;
        asm volatile(
            "btl  %2, (%1); "
            "sbbl %0, %0"
            : "=r" (tmp)
            : "r" (bits), "r" (i)
            : "cc");
        return tmp;
    }
#endif

    return (bits[BVINDEX(i)] & BVMASK(i)) != 0;
}

static inline void BitVector_SetRaw(int i, uint64 *bits)
{
#if defined(__GNUC__) && (defined(ARCH_AMD64) || defined(ARCH_x86))
    if (!CONSTANT(i)) {
        asm volatile(
            "btsl %1, (%0)"
            :: "r" (bits), "r" (i)
            : "cc", "memory");

        return;
    }
#endif
    bits[BVINDEX(i)] |= BVMASK(i);
}


static inline void BitVector_SetRaw64(int i, uint64 *bits)
{
    ASSERT(i >= 0);
    ASSERT(i < 64);
    *bits |= BVMASK(i);
}

static inline void BitVector_SetRaw32(int i, uint32 *bits)
{
    ASSERT(i >= 0);
    ASSERT(i < 32);
    *bits |= BVMASK(i);
}

static inline void BitVector_SetRaw16(int i, uint16 *bits)
{
    ASSERT(i >= 0);
    ASSERT(i < 16);
    *bits |= BVMASK(i);
}

static inline void BitVector_ResetRaw64(int i, uint64 *bits)
{
    ASSERT(i >= 0);
    ASSERT(i < 64);
    *bits &= ~BVMASK(i);
}

static inline void BitVector_ResetRaw32(int i, uint32 *bits)
{
    ASSERT(i >= 0);
    ASSERT(i < 32);
    *bits &= ~BVMASK(i);
}

static inline void BitVector_ResetRaw16(int i, uint16 *bits)
{
    ASSERT(i >= 0);
    ASSERT(i < 16);
    *bits &= ~BVMASK(i);
}


static inline void BitVector_ResetRaw(int i, uint64 *bits)
{
#if defined(__GNUC__) && (defined(ARCH_AMD64) || defined(ARCH_x86))
    if (!CONSTANT(i)) {
        asm volatile(
            "btrl %1, (%0)"
            :: "r" (bits), "r" (i)
            : "cc", "memory");
        return;
    }
#endif
    bits[BVINDEX(i)] &= ~BVMASK(i);
}

static inline void BitVector_FlipRaw(int i, uint64 *bits)
{
#if defined(__GNUC__) && (defined(ARCH_AMD64) || defined(ARCH_x86))
    if (!CONSTANT(i)) {
        asm volatile(
            "btcl %1, (%0)"
            :: "r" (bits), "r" (i)
            : "cc", "memory");

        return;
    }
#endif
    bits[BVINDEX(i)] ^= BVMASK(i);
}

static inline bool BitVector_Get(const CBitVector *b, int x)
{
    ASSERT(b != NULL);
    ASSERT(x >= 0);
    ASSERT((uint) x < b->size);

    return BitVector_GetRaw(x, BitVectorGetPtr(b));
}

static inline void BitVector_Put(BitVector *b, int x, bool v)
{
    ASSERT(b != NULL);
    ASSERT(x >= 0);
    ASSERT((uint) x < b->size);

    if (v) {
        BitVector_SetRaw(x, BitVectorGetPtr(b));
    } else {
        BitVector_ResetRaw(x, BitVectorGetPtr(b));
    }
}

static inline void BitVector_Set(BitVector *b, int x)
{
    ASSERT(b != NULL);
    ASSERT(x >= 0);
    ASSERT((uint) x < b->size);

    BitVector_SetRaw(x, BitVectorGetPtr(b));
}

static inline void BitVector_Reset(BitVector *b, int x)
{
    ASSERT(b != NULL);
    ASSERT(x >= 0);
    ASSERT((uint) x < b->size);

    BitVector_ResetRaw(x, BitVectorGetPtr(b));
}

static inline void BitVector_Flip(BitVector *b, int x)
{
    ASSERT(b != NULL);
    ASSERT(x >= 0);
    ASSERT((uint) x < b->size);

    BitVector_FlipRaw(x, BitVectorGetPtr(b));
}

static INLINE_ALWAYS void
BitVectorWrite(BitVector *b, int x, BitVectorWriteType type)
{
    switch (type) {
        case BITVECTOR_WRITE_SET:
            BitVector_Set(b, x);
            break;
        case BITVECTOR_WRITE_RESET:
            BitVector_Reset(b, x);
            break;
        case BITVECTOR_WRITE_FLIP:
            BitVector_Flip(b, x);
            break;
    }
}

static INLINE_ALWAYS void
BitVectorWriteRangeGenericDispatch(BitVector *b,
                                   int first, int last,
                                   BitVectorWriteType type)
{
    switch (type) {
        case BITVECTOR_WRITE_SET:
            BitVectorSetRangeGeneric(b, first, last);
            break;
        case BITVECTOR_WRITE_RESET:
            BitVectorResetRangeGeneric(b, first, last);
            break;
        case BITVECTOR_WRITE_FLIP:
            BitVectorFlipRangeGeneric(b, first, last);
            break;
    }
}

static INLINE_ALWAYS void
BitVectorWriteRangeOptimized(BitVector *b,
                             int first, int last,
                             BitVectorWriteType type)
{
    const uint8 alignment = 8;

    ASSERT(b != NULL);
    ASSERT(first % alignment == 0);
    ASSERT(last % alignment == (alignment - 1));
    ASSERT(last != first);

    int x;
    uint8 fillByte;
    uint8 *myBytes;
    int numBytes;
    uint32 startByte;

    numBytes = (last - first) / alignment + 1;
    myBytes = (uint8 *) BitVectorGetPtr(b);
    startByte = first / alignment;
    myBytes += startByte;

    ASSERT(startByte + numBytes <= b->arrSize * BVUNITBYTES);

    switch (type) {
        case BITVECTOR_WRITE_SET:
        case BITVECTOR_WRITE_RESET:
            if (type == BITVECTOR_WRITE_SET) {
                fillByte = 0xFF;
            } else {
                fillByte = 0x00;
            }

            memset(myBytes, fillByte, numBytes);
            break;
        case BITVECTOR_WRITE_FLIP:
            for (x = 0; x < numBytes; x++) {
                myBytes[x] = ~myBytes[x];
            }
            break;
    }
}

static INLINE_ALWAYS void
BitVectorWriteRangePartial(BitVector *b,
                           int first, int last, BitVectorWriteType type)
{
    ASSERT(b != NULL);
    ASSERT(last - first <= 16);

    if (CONSTANT(type) && CONSTANT(first) && CONSTANT(last)) {
        int count = last - first + 1;
        ASSERT(count >= 0);
        UNROLL(16, i, {
            if (i < (uint) count) {
                BitVectorWrite(b, first + i, type);
            }
        });
    } else {
        int x = first;
        while (x <= last) {
            BitVectorWrite(b, x, type);
            x++;
        }
    }
}

static INLINE_ALWAYS void
BitVectorWriteRangeGenericImpl(BitVector *b,
                               int first, int last,
                               BitVectorWriteType type)
{
    ASSERT(b != NULL);
    ASSERT(first >= 0);
    ASSERT((uint) first < b->size);
    ASSERT(last >= 0);
    ASSERT((uint) last < b->size);
    ASSERT(first <= last);

    const uint8 alignment = 8;
    int alignedLast;
    int alignedFirst;

    if (last - first + 1 <= alignment * 2) {
        BitVectorWriteRangePartial(b, first, last, type);
        return;
    }

    if (first % alignment != 0) {
        alignedFirst = first + (alignment - (first % alignment));
        ASSERT(first < alignedFirst);

        ASSERT(alignedFirst - 1 < last);
        BitVectorWriteRangePartial(b, first, alignedFirst - 1, type);

        first = alignedFirst;
        ASSERT(first <= last);
    }

    if (last % alignment != alignment - 1) {
        alignedLast = last - (last % alignment) - 1;
        ASSERT(alignedLast >= 0);
        ASSERT(alignedLast >= first);
        ASSERT(alignedLast < last);

        BitVectorWriteRangePartial(b, alignedLast + 1, last, type);
        ASSERT(alignedLast <= last);
        last = alignedLast;
    }

    ASSERT(first < last);
    ASSERT(first % alignment == 0);
    ASSERT(last % alignment == (alignment - 1));
    BitVectorWriteRangeOptimized(b, first, last, type);
    return;
}

static INLINE_ALWAYS void BitVectorWriteRange(BitVector *b, int first, int last,
                                              BitVectorWriteType type)
{
    ASSERT(b != NULL);
    ASSERT(first >= 0);
    ASSERT((uint) first < b->size);
    ASSERT(last >= 0);
    ASSERT((uint) last < b->size);
    ASSERT(first <= last);

    if (CONSTANT(type) && CONSTANT(first) && CONSTANT(last)) {
        BitVectorWriteRangeGenericImpl(b, first, last, type);
        return;
    } else {
        BitVectorWriteRangeGenericDispatch(b, first, last, type);
    }
}

static INLINE_ALWAYS void BitVector_SetAll(BitVector *b)
{
    int first = 0;
    int last = b->size - 1;

    BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_SET);
}

static INLINE_ALWAYS void BitVector_ResetAll(BitVector *b)
{
    int first = 0;
    int last = b->size - 1;

    BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_RESET);
}

static INLINE_ALWAYS void BitVector_FlipAll(BitVector *b)
{
    int first = 0;
    int last = b->size - 1;

    BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_FLIP);
}

static INLINE_ALWAYS void BitVector_SetRange(BitVector *b, int first, int last)
{
    BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_SET);
}

static INLINE_ALWAYS void BitVector_ResetRange(BitVector *b, int first,
                                               int last)
{
    BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_RESET);
}

static INLINE_ALWAYS void BitVector_FlipRange(BitVector *b, int first, int last)
{
    BitVectorWriteRange(b, first, last, BITVECTOR_WRITE_FLIP);
}

static inline bool BitVector_TestAndSet(BitVector *b, int x)
{
    bool oup;
    ASSERT(b != NULL);
    ASSERT(x >= 0);
    ASSERT((uint) x < b->size);

    oup = BitVector_GetRaw(x, BitVectorGetPtr(b));
    BitVector_SetRaw(x, BitVectorGetPtr(b));
    return oup;
}

static inline void BitVector_MakeEmpty(BitVector *b)
{
    ASSERT(b != NULL);
    b->size = 0;
}

static inline int BitVector_Size(const BitVector *b)
{
    ASSERT(b != NULL);
    return b->size;
}

static inline bool BitVector_GetFillValue(const BitVector *b)
{
    ASSERT(b != NULL);
    return b->fill;
}

static inline void BitVector_SetFillValue(BitVector *b, bool f)
{
    ASSERT(b != NULL);
    b->fill = f;
}

static inline int BitVector_PopCount(const BitVector *b)
{
    int x;
    int size;
    int cellSize;
    int sum;

    int strayBitCount;
    uint64 strayBitMask;

    ASSERT(b != NULL);

    sum = 0;
    size = b->size;
    cellSize = size / BVUNITBITS;

    for (x = 0; x < cellSize; x++) {
        sum += MBUtil_Popcountl(BitVectorGetPtr(b)[x]);
    }

    strayBitCount = size % BVUNITBITS;
    strayBitMask = (((uint64) 1) << strayBitCount) - 1;

    ASSERT(cellSize >= 0);
    ASSERT((uint) cellSize < b->arrSize);
    sum += MBUtil_Popcountl(BitVectorGetPtr(b)[cellSize] & strayBitMask);

    return sum;
}

#ifdef __cplusplus
    }
#endif

#endif //BitVector_H_201001192326
