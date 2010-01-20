
#ifndef TYPES_H_200910301741
#define TYPES_H_200910301741

#include "config.h"

#include <limits.h>

#ifndef NULL
	#define NULL 0
#endif

#ifndef null
	#define null 0
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef true
	#define true 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef false
	#define false 0
#endif

typedef bool boolean;

typedef unsigned char byte;

//autodetect this or require it?
#define WORD_BIT 32

#ifndef WORD_BIT
	#error Unable to determine word size
#endif

#if (WORD_BIT == 32)
	typedef unsigned int uint32;
	typedef int int32;
#else
	#error Unknown architecture size
#endif


#endif //TYPES_H_200910301741

