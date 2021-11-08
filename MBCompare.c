/*
 * MBCompare.c -- part of MBLib
 *
 * Copyright (c) 2020-2021 Michael Banack <github@banack.net>
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

#include <stdlib.h>

#include "MBCompare.h"
#include "MBAssert.h"

void
MBCompare_SortFallback(void *items, uint32 numItems, uint32 itemSize,
                       CMBCompareFn compareFn, void *cbData)
{
    if (numItems <= 1) {
        return;
    }

#define GET_ITEM(_n) (&((uint8 *)items)[itemSize * _n])
#define COMPARE(_lhs, _rhs) (compareFn(GET_ITEM(_lhs), GET_ITEM(_rhs), cbData))

    void *swapSpace = malloc(itemSize);

    ASSERT(numItems <= MAX_UINT32);

    for (uint32 i = 0; i < numItems; i++) {
        uint32 min = i;
        for (uint32 k = i + 1; k < numItems; k++) {
            if (COMPARE(min, k) > 0) {
                min = k;
            }
        }

        if (min != i) {
            memcpy(swapSpace, GET_ITEM(min), itemSize);
            memcpy(GET_ITEM(min), GET_ITEM(i), itemSize);
            memcpy(GET_ITEM(i), swapSpace ,itemSize);
        }
    }

    free(swapSpace);

#undef GET_ITEM
#undef COMPARE
}

int
MBCompare_FindMin(void *items, uint32 numItems, uint32 itemSize,
                  CMBCompareFn compareFn, void *cbData)
{
    if (numItems == 0) {
        return -1;
    } else if (numItems == 1) {
        return 0;
    }

#define GET_ITEM(_n) (&((uint8 *)items)[itemSize * _n])
#define COMPARE(_lhs, _rhs) (compareFn(GET_ITEM(_lhs), GET_ITEM(_rhs), cbData))

    ASSERT(numItems <= MAX_UINT32);

    uint32 min = 0;
    for (uint32 i = 1; i < numItems; i++) {
        if (COMPARE(min, i) > 0) {
            min = i;
        }
    }

    return min;

#undef GET_ITEM
#undef COMPARE
}
