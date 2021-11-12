/*
 * MBRing.c -- part of MBLib
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

#include "MBRing.h"

void MBRingResizeHelper(MBRing *ring)
{
    int oldRSize = MBRing_Size(ring);
    ASSERT(oldRSize > 0);

    CMBVector v;
    int itemSize = CMBVector_ItemSize(&ring->vector);
    CMBVector_CreateEmpty(&v, itemSize);
    CMBVector_Consume(&v, &ring->vector);
    int size = CMBVector_Size(&v);
    ASSERT(size > 0);
    ASSERT(size * 2 > size);
    CMBVector_Resize(&ring->vector, size * 2);

    if (ring->tail >= ring->head) {
        int numItems = ring->tail - ring->head;
        ASSERT(numItems > 0);
        int memSize = numItems * itemSize;
        ASSERT(memSize > 0);
        ASSERT(memSize >= numItems);
        ASSERT(memSize >= itemSize);
        memcpy(CMBVector_GetPtr(&ring->vector, 0),
                CMBVector_GetPtr(&v, ring->head), memSize);
    } else {
        int numItems = size - ring->head;
        ASSERT(numItems > 0);
        int memSize = numItems * itemSize;
        ASSERT(memSize > 0);
        ASSERT(memSize >= numItems);
        ASSERT(memSize >= itemSize);
        memcpy(CMBVector_GetPtr(&ring->vector, 0),
                CMBVector_GetPtr(&v, ring->head), memSize);

        int firstItems = numItems;
        numItems = ring->tail;
        if (numItems > 0) {
            memSize = numItems * itemSize;
            ASSERT(memSize > 0);
            ASSERT(memSize >= numItems);
            ASSERT(memSize >= itemSize);
            memcpy(CMBVector_GetPtr(&ring->vector, firstItems),
                   CMBVector_GetPtr(&v, 0), memSize);
        }
    }

    ring->head = 0;
    ring->tail = oldRSize;

    CMBVector_Destroy(&v);
}
