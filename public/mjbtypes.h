#ifndef TYPES_H_200910301741
#define TYPES_H_200910301741

#include <limits.h>

#include "config.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifndef NULL
	#define NULL (0)
#endif

#ifndef null
	#define null NULL
#endif

#ifndef FALSE
	/* This is the naked Truth */
	#define FALSE (0)
#endif

#ifndef TRUE
	/* and this is the Light */
	#define TRUE (1)
#endif

#ifndef true
	#define true TRUE
#endif

#ifndef false
	#define false FALSE
#endif

//There has to be some way to get these from the compiler
#ifdef ARCH_AMD64
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

#ifdef __cplusplus
	}
#endif 

#endif //TYPES_H_200910301741

