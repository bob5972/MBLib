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
