/*
 * MBRing.h -- part of MBLib
 *
 * Copyright (c) 2021 Michael Banack <github@banack.net>
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

#ifndef MBRING_H_20211111
#define MBRING_H_20211111

#ifdef __cplusplus
    extern "C" {
#endif

#include "MBVector.h"

typedef struct MBRing {
    CMBVector vector;
    uint head;
    uint tail;
} MBRing;

static INLINE void MBRing_Create(MBRing *ring, int itemSize)
{
    ASSERT(ring != NULL);
    ASSERT(itemSize > 0);
    CMBVector_CreateWithSize(&ring->vector, itemSize, 8);
    ring->head = 0;
    ring->tail = 0;
}

static INLINE void MBRing_Destroy(MBRing *ring)
{
    ASSERT(ring != NULL);
    CMBVector_Destroy(&ring->vector);
}

static INLINE void MBRing_MakeEmpty(MBRing *ring)
{
    ASSERT(ring != NULL);
    ring->head = 0;
    ring->tail = 0;
}

static INLINE int MBRing_Size(const MBRing *ring)
{
    if (ring->tail >= ring->head) {
        return ring->tail - ring->head;
    } else {
        return CMBVector_Size(&ring->vector) - ring->head + ring->tail;
    }
}

void MBRingResizeHelper(MBRing *ring);

static INLINE uint MBRingGetMask(const MBRing *ring)
{
    int size = CMBVector_Size(&ring->vector);
    ASSERT(size > 0);
    ASSERT(MBUtil_IsPow2(size));
    return ((uint)size) - 1;
}

static INLINE void MBRing_InsertHead(MBRing *ring, const void *item,
                                     uint itemSize)
{
    ASSERT(ring != NULL);
    ASSERT(itemSize == CMBVector_ItemSize(&ring->vector));

    if (MBRing_Size(ring) + 1 == CMBVector_Size(&ring->vector)) {
        MBRingResizeHelper(ring);
    }

    ring->head--;
    ring->head &= MBRingGetMask(ring);
    ASSERT(ring->tail != ring->head);

    void *dest = CMBVector_GetPtr(&ring->vector, ring->head);
    memcpy(dest, item, CMBVector_ItemSize(&ring->vector));
}

static INLINE void MBRing_InsertTail(MBRing *ring, const void *item,
                                     uint itemSize)
{
    ASSERT(ring != NULL);
    ASSERT(itemSize == CMBVector_ItemSize(&ring->vector));

    if (MBRing_Size(ring) + 1 == CMBVector_Size(&ring->vector)) {
        MBRingResizeHelper(ring);
    }

    void *dest = CMBVector_GetPtr(&ring->vector, ring->tail);
    memcpy(dest, item, CMBVector_ItemSize(&ring->vector));
    ring->tail++;
    ring->tail &= MBRingGetMask(ring);
    ASSERT(ring->tail != ring->head);
}

static INLINE void MBRing_RemoveHead(MBRing *ring, void *item, uint itemSize)
{
    ASSERT(ring != NULL);
    ASSERT(itemSize == CMBVector_ItemSize(&ring->vector));
    ASSERT(MBRing_Size(ring) > 0);

    void *src = CMBVector_GetPtr(&ring->vector, ring->head);
    memcpy(item, src, CMBVector_ItemSize(&ring->vector));

    ring->head++;
    ring->head &= MBRingGetMask(ring);
}

static INLINE void MBRing_RemoveTail(MBRing *ring, void *item, uint itemSize)
{
    ASSERT(ring != NULL);
    ASSERT(itemSize == CMBVector_ItemSize(&ring->vector));
    ASSERT(MBRing_Size(ring) > 0);

    int lastItem = (ring->tail - 1) & MBRingGetMask(ring);

    void *src = CMBVector_GetPtr(&ring->vector, lastItem);
    memcpy(item, src, CMBVector_ItemSize(&ring->vector));
    ring->tail = lastItem;
}

#ifdef __cplusplus
	}
#endif

#endif // MBRING_H_20211111