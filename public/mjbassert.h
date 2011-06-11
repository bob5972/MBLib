
#ifndef MJBASSERT_H_201001091250
#define MJBASSERT_H_201001091250

#include "config.h"
#include "mjbtypes.h"
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

	#define ASSERT(x) do { \
		if (!(x)) { \
			PanicHelper("ASSERT", __FILE__, __LINE__); \
		} \
	} while (FALSE)
	
#else
	#define ASSERT(x)
#endif

#define NOT_REACHED() { PanicHelper( __FILE__, __LINE__, "NOT_REACHED"); }

#define NOT_IMPLEMENTED() { PanicHelper( __FILE__, __LINE__, "NOT_IMPLEMENTED"); }

#define PANIC(msg) { PanicHelper( __FILE__, __LINE__, (msg)); }

void Panic(const char *fmt, ...);

void PanicHelper(const char *file, int line, const char *message);

#endif //MJBASSERT_H_201001091250
