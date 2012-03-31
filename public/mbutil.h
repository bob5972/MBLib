/*
 * mbutil.h --
 */
 
#ifndef _MBUTIL_H_201106111445
#define _MBUTIL_H_201106111445

#include <string.h>

#include "mbtypes.h"

#ifdef __cplusplus
	extern "C" {
#endif

#define MIN(x, y) ( (x) < (y) ? (x) : (y))
#define MAX(x, y) ( (x) > (y) ? (x) : (y))
#define CLAMP(min, val, max) ( val < min ? min  : ( val > max ? max : val))

#define IMPLIES(x, y) (!(x) || (y))
#define EQUIVALENT(x, y) (!!(x) == !!(y))

#define ARRAYSIZE(x) ((sizeof x) / (sizeof *x))

#define OFFSETOF(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define INLINE inline
#define EXTERN extern

//It might be possible to check #if HAVE_BUILTIN_EXPECT
//to determine when this is safe.
#define LIKELY(x) (__builtin_expect(!!(x), 1))
#define UNLIKELY(x) (__builtin_expect((x), 0))

#define CONSTANT(x) (__builtin_constant_p(x))

#define UNUSED_VARIABLE(x) (x) = (x)

static INLINE bool
isDigit(char c)
{
    return '0' <= c && c <= '9';
}

static INLINE bool
isWhitespace(char c)
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
zero(void *p, uint size)
{
	memset(p, 0, size);
}

static INLINE uint8
popcount(uint32 x)
{
	return __builtin_popcount(x);
}

#ifdef __cplusplus
	}
#endif 

#endif //_MBUTIL_H_201106111445
