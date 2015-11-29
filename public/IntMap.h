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
    BitVector myActiveFlags;
    BitVector myFullFlags;

    int mySize;
    int mySpace;
    int myFreeSpace;
    double myLoad;
} IntMap;

typedef IntMap IntMapData;

void IntMap_Create(IntMap *map);
void IntMap_Destroy(IntMap *map);

bool IntMap_ContainsKey(const IntMap *map, int key);
int IntMap_Get(const IntMap *map, int key);

void IntMap_MakeEmpty(IntMap *map);
int IntMap_IncrementBy(IntMap *map, int key, int amount);
void IntMap_Put(IntMap *map, int key, int value);
bool IntMap_Remove(IntMap *map, int key);
void IntMap_InsertAll(IntMap *dest, const IntMap *src);

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
