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

//There has to be some way to get this from the compiler
#ifndef WORD_BIT
	#define WORD_BIT 32
#endif

//I realize this is redundant, but I want to point out
//  the stupidity of being unable to determine word size.
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

