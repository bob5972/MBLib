/*
 * mbtypes.h -- part of MBLib
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
#ifndef _MBTYPES_H_200910301741
#define _MBTYPES_H_200910301741

#include <limits.h>

#include "config.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef NULL
    #define NULL (0)
#endif

#ifndef FALSE
    /* This is the naked Truth */
    #define FALSE (0)
#endif

#ifndef TRUE
    /* and this is the Light */
    #define TRUE (1)
#endif

#if defined(__x86_64__)
    #define ARCH_AMD64
#elif defined(__i386__)
    #define ARCH_x86
#endif

//There has to be some way to get these from the compiler
#if defined(ARCH_AMD64)
    #define SHORT_BIT 16
    #define WORD_BIT 32
#elif defined(ARCH_x86)
    #define SHORT_BIT 16
    #define WORD_BIT 32
#else
    #error Unknown architecture
#endif

#if (CHAR_BIT == 8)
    typedef char int8;
    typedef unsigned char uint8;
#else
    #error Unknown char size
#endif

#if (SHORT_BIT == 16)
    typedef short int16;
    typedef unsigned short uint16;
#else
    #error Unknown short size
#endif

#if (WORD_BIT == 32)
    typedef unsigned int uint32;
    typedef int int32;
    typedef long long int64;
    typedef unsigned long long uint64;
#else
	#error Unknown word size
#endif

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;


#ifndef __cplusplus
    /*
     * XXX: Need a clever way to make sure this is the same
     *      size as any corresponding C++ usage.
     *      ASSERT_ON_COMPILE ?
     */
    typedef uint8 bool;
#endif

#define MIN_UINT8 0
#define MAX_UINT8 255
#define MIN_UINT16 0
#define MAX_UINT16 65535
#define MIN_UINT32 0
#define MAX_UINT32 0xFFFFFFFF
#define MAX_INT32  0x7FFFFFFF
#define MIN_INT32  (-2147483648)

#if (WORD_BIT == 32)
#define MAX_UINT MAX_UINT32
#define MIN_UINT MIN_UINT32
#else
	#error Unknown word size
#endif

#ifdef __cplusplus
    }
#endif

#endif //_MBTYPES_H_200910301741
