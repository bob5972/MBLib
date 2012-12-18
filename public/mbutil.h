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

#define IMPLIES(x, y) (!(x) || (y))
#define EQUIVALENT(x, y) (!!(x) == !!(y))

#define ARRAYSIZE(x) ((sizeof x) / (sizeof *x))

#define OFFSETOF(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define INLINE inline
#define EXTERN extern

#define INLINE_ALWAYS INLINE __attribute__((__always_inline__))

#define NORETURN __attribute__((__noreturn__))

//It might be possible to check #if HAVE_BUILTIN_EXPECT
//to determine when this is safe.
#define LIKELY(x) (__builtin_expect(!!(x), 1))
#define UNLIKELY(x) (__builtin_expect((x), 0))

#define CONSTANT(x) (__builtin_constant_p(x))

#define UNUSED_VARIABLE(x) (x) = (x)

// Unroll a for loop from 0 to (n - 1).
#define UNROLL(_n, _i, _action) { UNROLL_ ## _n (_i, _action)   } 
#define UNROLL_0(_i, _action)
#define UNROLL_1(_i, _action)   UNROLL_0(_i, _action) ONCE(_i, 0, _action)
#define UNROLL_2(_i, _action)   UNROLL_1(_i, _action) ONCE(_i, 1, _action)
#define UNROLL_3(_i, _action)   UNROLL_2(_i, _action) ONCE(_i, 2, _action)
#define UNROLL_4(_i, _action)   UNROLL_3(_i, _action) ONCE(_i, 3, _action)
#define UNROLL_5(_i, _action)   UNROLL_4(_i, _action) ONCE(_i, 4, _action)
#define UNROLL_6(_i, _action)   UNROLL_5(_i, _action) ONCE(_i, 5, _action)
#define UNROLL_7(_i, _action)   UNROLL_6(_i, _action) ONCE(_i, 6, _action)
#define UNROLL_8(_i, _action)   UNROLL_7(_i, _action) ONCE(_i, 7, _action)
#define UNROLL_9(_i, _action)   UNROLL_8(_i, _action) ONCE(_i, 8, _action)
#define UNROLL_10(_i, _action)   UNROLL_9(_i, _action) ONCE(_i, 9, _action)
#define UNROLL_11(_i, _action)   UNROLL_10(_i, _action) ONCE(_i, 10, _action)
#define UNROLL_12(_i, _action)   UNROLL_11(_i, _action) ONCE(_i, 11, _action)
#define UNROLL_13(_i, _action)   UNROLL_12(_i, _action) ONCE(_i, 12, _action)
#define UNROLL_14(_i, _action)   UNROLL_13(_i, _action) ONCE(_i, 13, _action)
#define UNROLL_15(_i, _action)   UNROLL_14(_i, _action) ONCE(_i, 14, _action)
#define UNROLL_16(_i, _action)   UNROLL_15(_i, _action) ONCE(_i, 15, _action)

#define ONCE(_i, _v, _action) {    \
   const unsigned _i = (_v);       \
   unsigned _ignored ;             \
   _ignored = _i;                  \
   _ignored = _ignored;            \
   _action;                        \
}


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

static INLINE uint8
popcountl(uint64 x)
{
	return __builtin_popcountl(x);
}

#ifdef __cplusplus
	}
#endif 

#endif //_MBUTIL_H_201106111445
