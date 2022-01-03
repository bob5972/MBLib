/*
 * MBStrTable.h -- part of MBLib
 *
 * Copyright (c) 2022 Michael Banack <github@banack.net>
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

#ifndef _MBSTRTABLE_H_20220102
#define _MBSTRTABLE_H_20220102

#include "MBVector.h"

#ifdef __cplusplus
    extern "C" {
#endif

struct MBStrTable;
typedef struct MBStrTable MBStrTable;

MBStrTable *MBStrTable_Alloc();
void MBStrTable_Free(MBStrTable *st);

const char *MBStrTable_AddCopy(MBStrTable *st, const char *cstr);
void MBStrTable_AddFree(MBStrTable *st, const char *cstr);

#ifdef __cplusplus
    }
#endif

#endif // _MBSTRTABLE_H_20220102
