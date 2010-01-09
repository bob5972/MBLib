
#include <stdio.h>
#include <stdlib.h>

#include "mjbassert.h"



inline void AssertionHelper(int x, const char* file, int line)
{
	if (!x) {
		fprintf(stderr, "Assertion failure: %s:%d\n", file, line);
		exit(1);
	}
}

inline void PanicHelper(const char* message, const char* file, int line)
{
	fprintf(stderr, "Panic %s:%d: %s\n", file, line, message);
	exit(1);
}

inline void ErrorHelper(const char* message, const char* file, int line)
{
	fprintf(stderr, "Error %s:%d: %s\n", file, line, message);
	exit(1);
}
