/*
 * mbassert.h --
 */
 
#ifndef MBASSERT_H_201001091250
#define MBASSERT_H_201001091250

#include "config.h"
#include "mbtypes.h"
#include "mbdebug.h"
#include "mbutil.h"

#ifdef __cplusplus
	extern "C" {
#endif 

#if MB_DEBUG
	#ifndef MB_ASSERT
		#define MB_ASSERT 1
	#endif
#endif

#ifndef MB_ASSERT
	#define MB_ASSERT 0
#endif

#if MB_ASSERT
	#define ASSERT(x) do { \
		if (UNLIKELY(!(x))) { \
			PanicAssertFail(__FILE__, __LINE__, #x); \
		} \
	} while (FALSE)
	
#else
	#define ASSERT(x)
#endif

#define VERIFY(x) do { \
		if (UNLIKELY(!(x))) { \
			PanicWithMessage(__FILE__, __LINE__, "VERIFY FAILED"); \
		} \
	} while (FALSE)

#define NOT_REACHED() PanicWithMessage( __FILE__, __LINE__, "NOT_REACHED")

#define NOT_IMPLEMENTED() PanicWithMessage( __FILE__, __LINE__, "NOT_IMPLEMENTED")

#define PANIC(...) PanicWithMessage( __FILE__, __LINE__, __VA_ARGS__)

void PanicWithMessage(const char *file, int line, const char *fmt, ...);
void PanicAssertFail(const char *file, int line, const char *cond);
void Panic(void);

#ifdef __cplusplus
	}
#endif 

#endif //MBASSERT_H_201001091250
