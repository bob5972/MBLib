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

#define NOT_TESTED() DebugPrintHelper( __FILE__, __LINE__, "NOT_TESTED\n")

#define RTRACE() DebugPrintHelper( __FILE__, __LINE__, "TRACE: %s:%d\n", __FUNCTION__, __LINE__)

#if MB_DEBUG
	#define DebugPrint(...) DebugPrintHelper( __FILE__, __LINE__, __VA_ARGS__)
	#define TRACE() RTRACE()
    #define DEBUG_ONLY(x) x
#else
	#define DebugPrint(...)
	#define TRACE()
    #define DEBUG_ONLY(x)
#endif

void DebugPrintHelper(const char *file, int line, const char *fmt, ...);


#ifdef __cplusplus
	}
#endif

#endif //MBDEBUG_H_201001091239
