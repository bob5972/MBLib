/*
 * IntMap.h -- part of MBLib
 *
 * Copyright (c) 2015-2020 Michael Banack <github@banack.net>
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

#ifndef _INTMAP_H_20140106
#define _INTMAP_H_20140106

#include "MBVector.h"
#include "BitVector.h"

typedef struct CIntMap {
    CMBIntVec myKeys;
    CMBIntVec myValues;
    CBitVector myActiveFlags;
    CBitVector myFullFlags;

    int mySize;
    int mySpace;
    int myFreeSpace;
    int myFullSpace;
    int myTargetLoad;
    int myEmptyValue;
    uint myIndexMask;
} CIntMap;

typedef struct CIntMapIterator {
    int index;
    int used;
    CIntMap *map;
} CIntMapIterator;

void CIntMap_Create(CIntMap *map);
void CIntMap_Destroy(CIntMap *map);

void CIntMap_SetEmptyValue(CIntMap *map, int emptyValue);

bool CIntMap_ContainsKey(const CIntMap *map, int key);
int CIntMap_Get(const CIntMap *map, int key);
bool CIntMap_Lookup(const CIntMap *map, int key, int *value);

void CIntMap_MakeEmpty(CIntMap *map);
int CIntMap_IncrementBy(CIntMap *map, int key, int amount);
void CIntMap_Put(CIntMap *map, int key, int value);
bool CIntMap_Remove(CIntMap *map, int key);
void CIntMap_InsertAll(CIntMap *dest, const CIntMap *src);
void CIntMap_DebugDump(CIntMap *map);

static INLINE void CIntMapIterator_Start(CIntMapIterator *it, CIntMap *map)
{
    it->index = 0;
    it->used = 0;
    it->map = map;
}

static INLINE bool CIntMapIterator_HasNext(CIntMapIterator *it)
{
    return it->used < it->map->mySize;
}

static INLINE int CIntMapIterator_GetNext(CIntMapIterator *it)
{
    uint32 retInd;
    ASSERT(CIntMapIterator_HasNext(it));
    ASSERT(it->index < it->map->mySpace);

    while (!BitVector_Get(&it->map->myActiveFlags, it->index)) {
        it->index++;
        ASSERT(it->index < it->map->mySpace);
    }

    it->used++;
    retInd = it->index;
    it->index++;
    return CMBIntVec_GetValue(&it->map->myKeys, retInd);

}

static INLINE bool CIntMap_IsEmpty(const CIntMap *map)
{
    return map->mySize == 0;
}

static INLINE int CIntMap_Size(const CIntMap *map)
{
    return map->mySize;
}

// Returns the new value.
static INLINE int CIntMap_DecrementBy(CIntMap *map, int key, int amount)
{
    return CIntMap_IncrementBy(map, key, -amount);
}

// Returns the new value.
static INLINE int CIntMap_Increment(CIntMap *map, int key)
{
    return CIntMap_IncrementBy(map, key, 1);
}

// Returns the new value.
static INLINE int CIntMap_Decrement(CIntMap *map, int key)
{
    return CIntMap_DecrementBy(map, key, 1);
}

#endif // _INTMAP_H_20140106
