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
#endif //MB_ASSERT
#endif // MB_DEBUG

#ifndef MB_ASSERT
#define MB_ASSERT 0
#endif

#if MB_ASSERT
#define ASSERT(x)
    do { \
        if (UNLIKELY(!(x))) { \
                PanicAssertFail(__FILE__, __LINE__, #x); \
        } \
    } while (FALSE)
#else
#define ASSERT(x)
#endif

#define VERIFY(x) \
    do { \
        if (UNLIKELY(!(x))) { \
                PanicWithMessage(__FILE__, __LINE__, "VERIFY FAILED"); \
        } \
    } while (FALSE)

#define NOT_REACHED() PanicWithMessage( __FILE__, __LINE__, "NOT_REACHED")

#define NOT_IMPLEMENTED() PanicWithMessage( __FILE__, __LINE__, "NOT_IMPLEMENTED")

#define PANIC(...) PanicWithMessage( __FILE__, __LINE__, __VA_ARGS__)

NORETURN void PanicWithMessage(const char *file, int line, const char *fmt, ...);
NORETURN void PanicAssertFail(const char *file, int line, const char *cond);
NORETURN void Panic(void);

#ifdef __cplusplus
	}
#endif

#endif //MBASSERT_H_201001091250
