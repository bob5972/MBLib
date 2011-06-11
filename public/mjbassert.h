
#ifndef MJBASSERT_H_201001091250
#define MJBASSERT_H_201001091250

#include "config.h"
#include "mjbdebug.h"

#ifdef MJB_DEBUG
	#ifndef MJB_ASSERT
		#define MJB_ASSERT 1
	#endif
#endif

#ifndef MJB_ASSERT
	#define MJB_ASSERT 0
#endif

#if MJB_ASSERT	
	#define ASSERT(x) { AssertionHelper( (int)(x) , __FILE__, __LINE__); }
#else
	#define ASSERT(x)
#endif

#define NOT_REACHED() { PanicHelper( "NOT_REACHED", __FILE__, __LINE__); }

#define NOT_IMPLEMENTED() { PanicHelper( "NOT_IMPLEMENTED", __FILE__, __LINE__); }

#define PANIC(msg) { PanicHelper( (msg), __FILE__, __LINE__); }

#define ERROR(msg) { ErrorHelper( (msg), __FILE__, __LINE__); }

inline void AssertionHelper( int x , const char* file, int line);

inline void PanicHelper(const char* message, const char* file, int line);

inline void ErrorHelper(const char* message, const char* file, int line);

#include "mjbassert.cpp"

#endif //MJBASSERT_H_201001091250
