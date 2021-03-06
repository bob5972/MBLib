/*
 * MBRegistry.h -- part of MBLib
 *
 * Copyright (c) 2020 Michael Banack <github@banack.net>
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

#ifndef MBRegistry_H_202006121226
#define MBRegistry_H_202006121226

#include "mbtypes.h"
#include "mbbasic.h"

struct MBRegistry;
typedef struct MBRegistry MBRegistry;

MBRegistry *MBRegistry_Alloc();
MBRegistry *MBRegistry_AllocCopy(MBRegistry *toCopy);
void MBRegistry_Free(MBRegistry *mreg);

void MBRegistry_Load(MBRegistry *mreg, const char *filename);
void MBRegistry_LoadSubset(MBRegistry *mreg, const char *filename);
void MBRegistry_DebugDump(MBRegistry *mreg);

void MBRegistry_Put(MBRegistry *mreg, const char *key, const char *value);
bool MBRegistry_ContainsKey(MBRegistry *mreg, const char *key);
const char *MBRegistry_Get(MBRegistry *mreg, const char *key);
int MBRegistry_GetIntD(MBRegistry *mreg, const char *key, int defValue);
int64 MBRegistry_GetInt64D(MBRegistry *mreg, const char *key, int64 defValue);
uint MBRegistry_GetUintD(MBRegistry *mreg, const char *key, uint defValue);
uint64 MBRegistry_GetUint64D(MBRegistry *mreg, const char *key, uint64 defValue);
bool MBRegistry_GetBoolD(MBRegistry *mreg, const char *key, bool defValue);
float MBRegistry_GetFloatD(MBRegistry *mreg, const char *key, float defValue);

const char *MBRegistry_Remove(MBRegistry *mreg, const char *key);

MBRegistry *MBRegistry_Alloc(void);
MBRegistry *MBRegistry_AllocCopy(MBRegistry *toCopy);
void MBRegistry_Free(MBRegistry *mreg);

const char *MBRegistry_GetCStrD(MBRegistry *mreg, const char *key,
                                const char *defValue);

static INLINE const char *
MBRegistry_GetCStr(MBRegistry *mreg, const char *key)
{
    return MBRegistry_GetCStrD(mreg, key, NULL);
}
static INLINE bool MBRegistry_GetBool(MBRegistry *mreg, const char *key)
{
    return MBRegistry_GetBoolD(mreg, key, FALSE);
}
static INLINE int MBRegistry_GetInt(MBRegistry *mreg, const char *key)
{
    return MBRegistry_GetIntD(mreg, key, 0);
}
static INLINE int64 MBRegistry_GetInt64(MBRegistry *mreg, const char *key)
{
    return MBRegistry_GetInt64D(mreg, key, 0);
}
static INLINE uint MBRegistry_GetUint(MBRegistry *mreg, const char *key)
{
    return MBRegistry_GetUintD(mreg, key, 0);
}
static INLINE uint64 MBRegistry_GetUint64(MBRegistry *mreg, const char *key)
{
    return MBRegistry_GetUint64D(mreg, key, 0);
}

static INLINE float MBRegistry_GetFloat(MBRegistry *mreg, const char *key)
{
    return MBRegistry_GetFloatD(mreg, key, 0.0f);
}

#endif //MBRegistry_H_202006121226
