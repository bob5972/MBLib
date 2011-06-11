/*
 * mjbassert.cpp --
 */
 
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "mjbassert.h"

void Panic(const char *fmt, ...)
{
	va_list args;
	
	va_start(args, fmt);
	fprintf(stderr, "PANIC: ");
	vfprintf(stderr, fmt, args);
	va_end(args);
	
	exit(1);
}

void PanicHelper(const char *file, int line, const char *msg)
{
	DebugPrintHelper(file, line, msg);
	exit(1);
}


