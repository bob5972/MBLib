/*
 * MBConfig.h -- part of MBLib
 *
 * Copyright (c) 2021 Michael Banack <github@banack.net>
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

#ifndef _MBCONFIG_H_202101031218
#define _MBCONFIG_H_202101031218

#ifdef __cplusplus
	extern "C" {
#endif

#define ALLOW_MBBUILD_CONFIG_H
#include "config.h"
#undef ALLOW_MBBUILD_CONFIG_H

#ifdef MB_LINUX
#define mb_linux 1
#else
#define mb_linux 0
#endif

#ifdef MB_MACOS
#define mb_macos 1
#else
#define mb_macos 0
#endif

#ifdef MB_DEBUG
#define mb_debug 1
#else
#define mb_debug 0
#endif

#ifdef MB_DEVEL
#define mb_devel 1
#else
#define mb_devel 0
#endif

#ifdef MB_HAS_SDL2
#define mb_has_sdl2 1
#else
#define mb_has_sdl2 0
#endif

#ifdef __cplusplus
	}
#endif

#endif //_MBCONFIG_H_202101031218
