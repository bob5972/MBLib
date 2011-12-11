/*
 * mbdebug.h --
 */
 
#ifndef MBDEBUG_H_201001091239
#define MBDEBUG_H_201001091239

#include "config.h"

#ifdef __cplusplus
	extern "C" {
#endif 


//backtraces and logfile not implemented

#ifdef DEBUG
	#if DEBUG
		#define MB_DEBUG 1
	#endif
#endif

#ifndef MB_DEBUG
	#define MB_DEBUG 0
#endif

void Warning(const char *fmt, ...);
void Log(int level, const char *fmt, ...);

#define NOT_TESTED() { DebugPrintHelper( __FILE__, __LINE__, "NOT_TESTED\n"); }

#if MB_DEBUG
	#define DebugPrint(...) { DebugPrintHelper( __FILE__, __LINE__, __VA_ARGS__); }
	#define TRACE() { DebugPrintHelper( __FILE__, __LINE__, "TRACE: %s", __FUNCTION__); }
#else
	#define DebugPrint(msg) ;
	#define TRACE() ;
#endif

void DebugPrintHelper(const char *file, int line, const char *fmt, ...);


#ifdef __cplusplus
	}
#endif 

#endif //MBDEBUG_H_201001091239
