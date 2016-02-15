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
 * mbtypes.h --
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
#define MAX_UINT32 4294967295

#ifdef __cplusplus
	}
#endif 

#endif //_MBTYPES_H_200910301741

