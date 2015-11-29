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
 * mbutil.h --
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
