/*
 * mbbasic.h -- part of MBLib
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

#ifndef _MBBASIC_H_201402091412
#define _MBBASIC_H_201402091412

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#define IMPLIES(x, y) (!(x) || (y))
#define EQUIVALENT(x, y) (!!(x) == !!(y))

#define ARRAYSIZE(x) ((sizeof x) / (sizeof *x))

#define OFFSETOF(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define INLINE inline
#define EXTERN extern

#define INLINE_ALWAYS inline __attribute__((__always_inline__))

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

/*
 * Use _ignored to avoid unused variable warning if
 * the loop variable isn't actually used.
 */
#define ONCE(_i, _v, _action) {    \
   const unsigned _i = (_v);       \
   unsigned _ignored;              \
   _ignored = _i;                  \
   _ignored = _ignored;            \
   _action;                        \
}

#define KB_TO_BYTES(x) ((x) * 1024)
#define BYTES_TO_KB(x) ((x) / 1024)

#define MB_TO_BYTES(x) ((x) * 1024 * 1024)
#define BYTES_TO_MB(x) ((x) / 1024 / 1024)

#ifdef __cplusplus
	}
#endif

#endif //_MBBASIC_H_201402091412
