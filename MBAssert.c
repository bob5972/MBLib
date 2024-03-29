/*
 * MBAssert.c -- part of MBLib
 *
 * Copyright (c) 2015-2021 Michael Banack <github@banack.net>
 *
 * MIT License
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#include "MBAssert.h"
#include "MBDebug.h"

NORETURN void PanicWithMessage(const char *file, int line, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "%s:%d| ", file, line);
    fprintf(stderr, "PANIC: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    PanicExit();
}

NORETURN void PanicAssertFail(const char *file, int line, const char *cond)
{
    PanicWithMessage(file, line, "ASSERT: %s\n", cond);
}

NORETURN void PanicVerifyFail(const char *file, int line, const char *cond)
{
    PanicWithMessage(file, line, "VERIFY %s\n", cond);
}

NORETURN void PanicExit()
{
    int nptrs;
    void *buffer[100];
    char **symbols;

    /*
     * The project may need to be linked with -rdynamic in order to get
     * symbolic backtraces to work.
     */
    nptrs = backtrace(buffer, ARRAYSIZE(buffer));
    symbols = backtrace_symbols(buffer, nptrs);

    Warning("\n");
    Warning("Backtrace:\n");
    for (uint32 i = 0; i < nptrs; i++) {
        Warning("\t%s\n", symbols[i]);
    }

    exit(1);
}
