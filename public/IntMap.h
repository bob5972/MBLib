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
 * IntMap.h --
 */

#ifndef _INTMAP_H_20140106
#define _INTMAP_H_20140106

#include "MBVector.h"
#include "BitVector.h"

typedef struct IntMap {
    MBIntVector myKeys;
    MBIntVector myValues;
    CBitVector myActiveFlags;
    CBitVector myFullFlags;

    int mySize;
    int mySpace;
    int myFreeSpace;
    double myLoad;
} IntMap;

typedef IntMap IntMapData;

typedef struct IntMapIterator {
    int index;
    int used;
    IntMap *map;
} IntMapIterator;

void IntMap_Create(IntMap *map);
void IntMap_Destroy(IntMap *map);

bool IntMap_ContainsKey(const IntMap *map, int key);
int IntMap_Get(const IntMap *map, int key);

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
    return MBIntVector_GetValue(&it->map->myKeys, retInd);

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
