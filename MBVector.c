/*
 * MBVector.c -- part of MBLib
 *
 * Copyright (c) 2015-2020 Michael Banack <github@banack.net>
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

#include "MBVector.h"
#include <stdlib.h>

void CMBVector_EnsureCapacity(CMBVector *vector, int capacity)
{
    uint32 newCap;
    uint32 oldCapacity;
    uint32 newSize;
    void *newItems;

    ASSERT(vector->capacity > 0);
    ASSERT(vector->size >= 0);
    ASSERT(capacity >= 0);
    ASSERT(vector->itemSize > 0);

    if (vector->capacity >= capacity) {
        return;
    }

    ASSERT(vector->capacity < MAX_INT32 / 2);
    ASSERT(capacity < MAX_INT32 / 2);
    oldCapacity = vector->capacity;

    newCap = MAX(capacity, vector->capacity * 2);
    ASSERT(newCap > vector->capacity);
    vector->capacity = newCap;

    newSize = newCap * vector->itemSize;
    ASSERT(newSize > newCap);
    ASSERT(newSize > vector->itemSize);

    ASSERT(vector->pinCount == 0);
    newItems = realloc(vector->items, newSize);
    if (newItems != NULL) {
        vector->items = newItems;
    } else {
        PANIC("Unable to resize CMBVector: out of memory "
              "(oldSize=%d, newSize=%d)", oldCapacity, capacity);
    }
}
