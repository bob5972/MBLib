/*
 * mbassert.c --
 */
 
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "mbassert.h"
#include "mbdebug.h"

NORETURN void PanicWithMessage(const char *file, int line, const char *fmt, ...)
{
	va_list args;
	
	va_start(args, fmt);
	fprintf(stderr, "%s:%d| ", file, line);
	fprintf(stderr, "PANIC: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	Panic();
}

NORETURN void PanicAssertFail(const char *file, int line, const char *cond)
{
	fprintf(stderr, "%s:%d| ", file, line);
	fprintf(stderr, "ASSERT: %s\n", cond);
	Panic();
}

NORETURN void Panic(void)
{
	exit(1);
}


