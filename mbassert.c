/*
 * This file is part of MBLib.
 * Copyright (c) 2015 Michael Banack <bob5972@banack.net>
 *
 * MBLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MBLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MBLib.  If not, see <http://www.gnu.org/licenses/>.
 */

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


