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

typedef struct IntMap {
    CMBIntVec myKeys;
    CMBIntVec myValues;
    CBitVector myActiveFlags;
    CBitVector myFullFlags;

    int mySize;
    int mySpace;
    int myFreeSpace;
    int myFullSpace;
    double myLoad;
    int myEmptyValue;
} IntMap;

typedef IntMap IntMapData;

typedef struct IntMapIterator {
    int index;
    int used;
    IntMap *map;
} IntMapIterator;

void IntMap_Create(IntMap *map);
void IntMap_Destroy(IntMap *map);

void IntMap_SetEmptyValue(IntMap *map, int emptyValue);

bool IntMap_ContainsKey(const IntMap *map, int key);
int IntMap_Get(const IntMap *map, int key);
bool IntMap_Lookup(const IntMap *map, int key, int *value);

void IntMap_MakeEmpty(IntMap *map);
int IntMap_IncrementBy(IntMap *map, int key, int amount);
void IntMap_Put(IntMap *map, int key, int value);
bool IntMap_Remove(IntMap *map, int key);
void IntMap_InsertAll(IntMap *dest, const IntMap *src);
void IntMap_DebugDump(IntMap *map);

static INLINE void IntMapIterator_Start(IntMapIterator *it, IntMap *map)
{
    it->index = 0;
    it->used = 0;
    it->map = map;
}

static INLINE bool IntMapIterator_HasNext(IntMapIterator *it)
{
    return it->used < it->map->mySize;
}

static INLINE int IntMapIterator_GetNext(IntMapIterator *it)
{
    uint32 retInd;
    ASSERT(IntMapIterator_HasNext(it));
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

static INLINE bool IntMap_IsEmpty(const IntMap *map)
{
    return map->mySize == 0;
}

static INLINE int IntMap_Size(const IntMap *map)
{
    return map->mySize;
}

// Returns the new value.
static INLINE int IntMap_DecrementBy(IntMap *map, int key, int amount)
{
    return IntMap_IncrementBy(map, key, -amount);
}

// Returns the new value.
static INLINE int IntMap_Increment(IntMap *map, int key)
{
    return IntMap_IncrementBy(map, key, 1);
}

// Returns the new value.
static INLINE int IntMap_Decrement(IntMap *map, int key)
{
    return IntMap_DecrementBy(map, key, 1);
}

#endif // _INTMAP_H_20140106
