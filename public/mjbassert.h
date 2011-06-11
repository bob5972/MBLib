
#ifndef MJBASSERT_H_201001091250
#define MJBASSERT_H_201001091250

#include "config.h"
#include "mjbdebug.h"

#ifdef MJB_ENABLE_ASSERTIONS
	#ifdef MJB_DISABLE_ASSERTIONS
		#warning Conflicting assertion behavior specified.  Defaulting to enabled.
	#endif
	
	#define ASSERT(x) { AssertionHelper( (int)(x) , __FILE__, __LINE__); }
#else
	#ifndef MJB_DISABLE_ASSERTIONS
		#warning No assertion behavior specified.  Defaulting to disabled.
	#endif
	
	#define ASSERT(x)
#endif

#define NOT_REACHED() { PanicHelper( "Not Reached", __FILE__, __LINE__); }

#define NOT_IMPLEMENTED() { PanicHelper( "Not Implemented", __FILE__, __LINE__); }

#define PANIC(msg) { PanicHelper( (msg), __FILE__, __LINE__); }

#define ERROR(msg) { ErrorHelper( (msg), __FILE__, __LINE__); }

inline void AssertionHelper( int x , const char* file, int line);

inline void PanicHelper(const char* message, const char* file, int line);

inline void ErrorHelper(const char* message, const char* file, int line);

#include "mjbassert.cpp"

#endif //MJBASSERT_H_201001091250
