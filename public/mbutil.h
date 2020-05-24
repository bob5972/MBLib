/*
 * mbutil.h -- part of MBLib
 *
 * Copyright (c) 2015-2020 Michael Banack <bob5972@banack.net>
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

#include "mbbasic.h"
#include "mbtypes.h"

#ifdef __cplusplus
	extern "C" {
#endif

static INLINE bool
Util_IsDigit(char c)
{
    return '0' <= c && c <= '9';
}

static INLINE bool
Util_IsWhitespace(char c)
{
    switch (c) {
        case ' ':
        case '\t':
        case '\n':
            return TRUE;
        default:
            return FALSE;
    }
}

static INLINE void
Util_Zero(void *p, uint size)
{
	memset(p, 0, size);
}

static INLINE uint8
Util_Popcount(uint32 x)
{
	return __builtin_popcount(x);
}

static INLINE uint8
Util_Popcountl(uint64 x)
{
	return __builtin_popcountl(x);
}

#ifdef __cplusplus
	}
#endif 

#endif //_MBUTIL_H_201106111445
