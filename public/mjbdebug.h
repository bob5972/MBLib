#ifndef MJBDEBUG_H_201001091239
#define MJBDEBUG_H_201001091239

#include "config.h"

//backtraces and logfile not implemented

#ifdef DEBUG
	#define MJB_DEBUG 1
#endif

void Warning(const char *fmt, ...);
void Log(int level, const char *fmt, ...);

#define LOG(msg) Log(0, msg)

#ifdef DEBUG
	#define DebugPrint(msg) { DebugPrintHelper( __FILE__, __LINE__, msg); }
#else
	#define DebugPrint(msg)
#endif


#endif //MJBDEBUG_H_201001091239
