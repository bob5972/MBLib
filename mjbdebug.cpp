
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mjbdebug.h"

static int curLogLevel = 1;

void Warning(const char *fmt, ...)
{
	va_list args;
	
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}

//TODO: Make this go to a file.
void Log(int level, const char *fmt, ...)
{
	va_list args;
   
	if (level > curLogLevel) {
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		va_end(args);
	}
}

void DebugPrintHelper(const char *file, int line, const char *fmt, ...)
{
	const int bufSize = 1024;
	char buffer[bufSize];
	int len;
	va_list args;
	
	len = snprintf(buffer, bufSize, "%s:%d| ", file, line);
	va_start(args, fmt);
	len = vsnprintf(buffer + len, bufSize-len, fmt, args);
	va_end(args);
	
	Warning(buffer);
}


