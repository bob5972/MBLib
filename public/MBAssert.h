/*
 * MBAssert.h -- part of MBLib
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

#ifndef MBASSERT_H_201001091250
#define MBASSERT_H_201001091250

#include "MBConfig.h"
#include "MBTypes.h"
#include "MBDebug.h"
#include "MBUtil.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef MB_DEBUG
#define ASSERT(x) \
    do { \
        if (UNLIKELY(!(x))) { \
                PanicAssertFail(__FILE__, __LINE__, #x); \
        } \
    } while (FALSE)
#else
#define ASSERT(x)
#endif

#ifdef MB_DEBUG
#define VERIFY(x) \
    do { \
        if (UNLIKELY(!(x))) { \
            PanicVerifyFail(__FILE__, __LINE__, #x); \
        } \
    } while (FALSE)
#else
#define VERIFY(x) \
    do { \
        if (UNLIKELY(!(x))) { \
            PanicVerifyFail(__FILE__, __LINE__, "FAILED"); \
        } \
    } while (FALSE)
#endif

#ifdef MB_DEBUG
#define NOT_REACHED() \
    PanicWithMessage( __FILE__, __LINE__, "NOT_REACHED")
#else
#define NOT_REACHED() \
    __builtin_unreachable()
#endif

#define NOT_IMPLEMENTED() \
    do { \
        PanicWithMessage( __FILE__, __LINE__, "NOT_IMPLEMENTED"); \
        NOT_REACHED(); \
    } while (FALSE)

#define PANIC(...) PanicWithMessage( __FILE__, __LINE__, __VA_ARGS__)

NORETURN void PanicWithMessage(const char *file, int line, const char *fmt, ...)
              __attribute__((__format__ (__printf__, 3, 4)));
NORETURN void PanicAssertFail(const char *file, int line, const char *cond);
NORETURN void PanicVerifyFail(const char *file, int line, const char *cond);
NORETURN void PanicExit();

#ifdef __cplusplus
	}
#endif

#endif //MBASSERT_H_201001091250
