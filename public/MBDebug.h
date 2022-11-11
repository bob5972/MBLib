/*
 * MBDebug.h -- part of MBLib
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

#ifndef MBDEBUG_H_201001091239
#define MBDEBUG_H_201001091239

#include "MBBasic.h"

#ifdef __cplusplus
	extern "C" {
#endif


//backtraces and logfile not implemented

void Warning(const char *fmt, ...)
	__attribute__((__format__ (__printf__, 1, 2)));

#define NOT_TESTED() DebugPrintHelper( __FILE__, __LINE__, "NOT_TESTED\n")

#define RTRACE() DebugPrintHelper( __FILE__, __LINE__, "TRACE: %s:%d\n", __FUNCTION__, __LINE__)

#ifdef MB_DEBUG
	#define DebugPrint(...) DebugPrintHelper( __FILE__, __LINE__, __VA_ARGS__)
	#define TRACE() RTRACE()
    #define DEBUG_ONLY(x) x
#else
	#define DebugPrint(...)
	#define TRACE()
    #define DEBUG_ONLY(x)
#endif

void DebugPrintHelper(const char *file, int line, const char *fmt, ...);


#ifdef __cplusplus
	}
#endif

#endif //MBDEBUG_H_201001091239
