#ifndef MJBDEBUG_H_201001091239
#define MJBDEBUG_H_201001091239

#include "config.h"

#ifdef __cplusplus
	extern "C" {
#endif 


//backtraces and logfile not implemented

#ifdef DEBUG
	#define MJB_DEBUG 1
#endif

void Warning(const char *fmt, ...);
void Log(int level, const char *fmt, ...);

#define LOG(msg) Log(0, msg)

#ifdef DEBUG
	#define DebugPrint(msg) { DebugPrintHelper( __FILE__, __LINE__, msg); }
	#define TRACE() { DebugPrintHelper( __FILE__, __LINE__, "TRACE"); }
	#define NOT_TESTED() { DebugPrintHelper( __FILE__, __LINE__, "NOT_TESTED"); }
#else
	#define DebugPrint(msg) ;
	#define TRACE() ;
	#define NOT_TESTED() ;
#endif

void DebugPrintHelper(const char *file, int line, const char *fmt, ...);


#ifdef __cplusplus
	}
#endif 

#endif //MJBDEBUG_H_201001091239
