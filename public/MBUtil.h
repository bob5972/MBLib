/*
 * MBUtil.h -- part of MBLib
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

#ifndef _MBUTIL_H_201106111445
#define _MBUTIL_H_201106111445

#include <string.h>
#include <stdlib.h>

#include "MBBasic.h"
#include "MBTypes.h"

#ifdef __cplusplus
	extern "C" {
#endif

static INLINE bool
MBUtil_IsDigit(char c)
{
    return '0' <= c && c <= '9';
}

static INLINE bool
MBUtil_IsWhitespace(char c)
{
    switch (c) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            return TRUE;
        default:
            return FALSE;
    }
}

static INLINE void
MBUtil_Zero(void *p, uint size)
{
	memset(p, 0, size);
}

static INLINE bool
MBUtil_IsZero(void *p, uint size)
{
    uint8 *byte = (uint8 *)p;
    while (size > 0) {
        if (*byte != 0) {
            return FALSE;
        }
        byte++;
        size--;
    }
    return TRUE;
}

static INLINE void *
MBUtil_ZAlloc(uint size)
{
    return calloc(1, size);
}

static INLINE uint8
MBUtil_Popcount(uint32 x)
{
	return __builtin_popcount(x);
}

static INLINE uint8
MBUtil_Popcountl(uint64 x)
{
	return __builtin_popcountl(x);
}

#ifdef __cplusplus
	}
#endif

#endif //_MBUTIL_H_201106111445
