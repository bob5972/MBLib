/*
 * MBVarMap.h -- part of MBLib
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

#ifndef _MBINTVARMAP_H_20211207
#define _MBINTVARMAP_H_20211207

#include "MBVector.h"
#include "BitVector.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct CMBVarMap {
    CMBVarVec myKeys;
    CMBVarVec myValues;
    CBitVector myFlags;

    int mySize;
    int mySpace;
    int myFreeSpace;
    int myFullSpace;
    int myTargetLoad;
    MBVar myEmptyValue;
    uint myIndexMask;
} CMBVarMap;

typedef struct CMBVarMapIterator {
    int index;
    int used;
    CMBVarMap *map;
} CMBVarMapIterator;

typedef CMBVarMap CMBIntMap;
typedef CMBVarMapIterator CMBIntMapIterator;

void CMBVarMap_Create(CMBVarMap *map);
void CMBVarMap_Destroy(CMBVarMap *map);

void CMBVarMap_SetEmptyValue(CMBVarMap *map, MBVar emptyValue);

bool CMBVarMap_ContainsKey(const CMBVarMap *map, MBVar key);
MBVar CMBVarMap_Get(const CMBVarMap *map, MBVar key);
bool CMBVarMap_Lookup(const CMBVarMap *map, MBVar key, MBVar *value);

void CMBVarMap_MakeEmpty(CMBVarMap *map);
int CMBVarMap_IncrementByInt32(CMBVarMap *map, MBVar key, int32 amount);
void CMBVarMap_Put(CMBVarMap *map, MBVar key, MBVar value);
bool CMBVarMap_Remove(CMBVarMap *map, MBVar key);
void CMBVarMap_InsertAll(CMBVarMap *dest, const CMBVarMap *src);
void CMBVarMap_DebugDump(const CMBVarMap *map);

static INLINE bool CMBVarMapGetFullFlag(const CMBVarMap *map, int x)
{
    return BitVector_Get(&map->myFlags, x * 2 + 0);
}

static INLINE void CMBVarMapSetFullFlag(CMBVarMap *map, int x)
{
    BitVector_Set(&map->myFlags, x * 2 + 0);
}

static INLINE bool CMBVarMapGetActiveFlag(const CMBVarMap *map, int x)
{
    return BitVector_Get(&map->myFlags, x * 2 + 1);
}

static INLINE void CMBVarMapResetActiveFlag(CMBVarMap *map, int x)
{
    BitVector_Reset(&map->myFlags, x * 2 + 1);
}

static INLINE void CMBVarMapSetActiveFlag(CMBVarMap *map, int x)
{
    BitVector_Set(&map->myFlags, x * 2 + 1);
}

static INLINE void CMBVarMapIterator_Start(CMBVarMapIterator *it,
                                           CMBVarMap *map)
{
    it->index = 0;
    it->used = 0;
    it->map = map;
}

static INLINE bool CMBVarMapIterator_HasNext(CMBVarMapIterator *it)
{
    return it->used < it->map->mySize;
}

static INLINE MBVar CMBVarMapIterator_GetNext(CMBVarMapIterator *it)
{
    uint32 retInd;
    ASSERT(CMBVarMapIterator_HasNext(it));
    ASSERT(it->index < it->map->mySpace);

    while (!CMBVarMapGetActiveFlag(it->map, it->index)) {
        it->index++;
        ASSERT(it->index < it->map->mySpace);
    }

    it->used++;
    retInd = it->index;
    it->index++;
    return CMBVarVec_GetValue(&it->map->myKeys, retInd);
}

static INLINE bool CMBVarMap_IsEmpty(const CMBVarMap *map)
{
    return map->mySize == 0;
}

static INLINE int CMBVarMap_Size(const CMBVarMap *map)
{
    return map->mySize;
}

// Returns the new value.
static INLINE int CMBVarMap_DecrementByInt32(CMBVarMap *map, MBVar key, int32 amount)
{
    return CMBVarMap_IncrementByInt32(map, key, -amount);
}

/*
 * CMBIntMap --
 */
static INLINE void CMBIntMap_Create(CMBIntMap *map)
{
    VERIFY(sizeof(int) == sizeof(int32));
    CMBVarMap_Create(map);
}

static INLINE void CMBIntMap_Destroy(CMBIntMap *map)
{
    CMBVarMap_Destroy(map);
}

static INLINE void CMBIntMap_SetEmptyValue(CMBIntMap *map, int emptyValue)
{
    MBVar empty = {0};
    empty.vInt32 = emptyValue;
    CMBVarMap_SetEmptyValue(map, empty);
}

static INLINE bool CMBIntMap_ContainsKey(const CMBIntMap *map, int key)
{
    MBVar vkey = {0};
    vkey.vInt32 = key;
    return CMBVarMap_ContainsKey(map, vkey);
}

static INLINE int CMBIntMap_Get(const CMBIntMap *map, int key)
{
    MBVar vkey = {0};
    MBVar vvalue = {0};
    vkey.vInt32 = key;
    vvalue = CMBVarMap_Get(map, vkey);
    return vvalue.vInt32;
}

static INLINE bool CMBIntMap_Lookup(const CMBIntMap *map, int key, int *value)
{
    MBVar vkey = {0};
    MBVar vvar = {0};
    bool retVal;
    vkey.vInt32 = key;
    vvar.vInt32 = *value;

    retVal = CMBVarMap_Lookup(map, vkey, &vvar);
    *value = vvar.vInt32;

    return retVal;
}

static INLINE void CMBIntMap_MakeEmpty(CMBIntMap *map)
{
    CMBVarMap_MakeEmpty(map);
}

static INLINE int CMBIntMap_IncrementBy(CMBIntMap *map, int key, int amount)
{
    MBVar vkey = {0};
    vkey.vInt32 = key;

    return CMBVarMap_IncrementByInt32(map, vkey, amount);
}

static INLINE void CMBIntMap_Put(CMBIntMap *map, int key, int value)
{
    MBVar vkey = {0};
    MBVar vvar = {0};
    vkey.vInt32 = key;
    vvar.vInt32 = value;

    CMBVarMap_Put(map, vkey, vvar);
}

static INLINE bool CMBIntMap_Remove(CMBIntMap *map, int key)
{
    MBVar vkey = {0};
    vkey.vInt32 = key;

    return CMBVarMap_Remove(map, vkey);
}

static INLINE void CMBIntMap_InsertAll(CMBIntMap *dest, const CMBIntMap *src)
{
    CMBVarMap_InsertAll(dest, src);
}

static INLINE void CMBIntMap_DebugDump(const CMBIntMap *map)
{
    CMBVarMap_DebugDump(map);
}

static INLINE void
CMBIntMapIterator_Start(CMBIntMapIterator *it, CMBIntMap *map)
{
    CMBVarMapIterator_Start(it, map);
}

static INLINE bool CMBIntMapIterator_HasNext(CMBIntMapIterator *it)
{
    return CMBVarMapIterator_HasNext(it);
}

static INLINE int CMBIntMapIterator_GetNext(CMBIntMapIterator *it)
{
    MBVar vvalue = {0};
    vvalue.vInt32 = CMBIntMapIterator_GetNext(it);
    return vvalue.vInt32;
}

static INLINE bool CMBIntMap_IsEmpty(const CMBIntMap *map)
{
    return CMBVarMap_IsEmpty(map);
}

static INLINE int CMBIntMap_Size(const CMBIntMap *map)
{
    return CMBVarMap_Size(map);
}

// Returns the new value.
static INLINE int CMBIntMap_DecrementBy(CMBIntMap *map, int key, int amount)
{
    return CMBIntMap_IncrementBy(map, key, -amount);
}

// Returns the new value.
static INLINE int CMBIntMap_Increment(CMBIntMap *map, int key)
{
    return CMBIntMap_IncrementBy(map, key, 1);
}

// Returns the new value.
static INLINE int CMBIntMap_Decrement(CMBIntMap *map, int key)
{
    return CMBIntMap_IncrementBy(map, key, -1);
}

#ifdef __cplusplus
    }
#endif

#endif // _MBINTVARMAP_H_20211207
