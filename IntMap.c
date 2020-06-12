/*
 * IntMap.c -- part of MBLib
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

#include "IntMap.h"

#define DEFAULT_SPACE 15
#define DEFAULT_LOAD  (0.60)
#define SEARCH_INCR   2

static int IntMapFindKey(const IntMap *map, int key);
static int IntMapGetInsertionIndex(const IntMap *map, int key);
static int IntMapHash(const IntMap *map, int key);
static void IntMapPutHelper(IntMap *map, int index, int key, int value);
static void IntMapRehash(IntMap *map);


void IntMap_Create(IntMap *map)
{
    MBIntVector_CreateWithSize(&map->myKeys, DEFAULT_SPACE);
    MBIntVector_CreateWithSize(&map->myValues, DEFAULT_SPACE);

    BitVector_CreateWithSize(&map->myActiveFlags, DEFAULT_SPACE);
    ASSERT(BitVector_GetFillValue(&map->myActiveFlags) == FALSE);
    BitVector_CreateWithSize(&map->myFullFlags, DEFAULT_SPACE);
    ASSERT(BitVector_GetFillValue(&map->myFullFlags) == FALSE);

    map->mySize = 0;
    map->mySpace = DEFAULT_SPACE;
    map->myFreeSpace = DEFAULT_SPACE;
    map->myFullSpace = 0;
    map->myLoad = DEFAULT_LOAD;

    // Zero is the default emptyValue.
    map->myEmptyValue = 0;
}

void IntMap_Destroy(IntMap *map)
{
    MBIntVector_Destroy(&map->myKeys);
    MBIntVector_Destroy(&map->myValues);
    BitVector_Destroy(&map->myActiveFlags);
    BitVector_Destroy(&map->myFullFlags);
}

void IntMap_SetEmptyValue(IntMap *map, int emptyValue)
{
    map->myEmptyValue = emptyValue;
}

void IntMap_MakeEmpty(IntMap *map)
{
    BitVector_ResetAll(&map->myFullFlags);
    BitVector_ResetAll(&map->myActiveFlags);
    map->mySize = 0;
    map->myFullSpace = 0;
    map->myFreeSpace = map->mySpace;
}

bool IntMap_ContainsKey(const IntMap *map, int key)
{
    return IntMapFindKey(map, key) != -1;
}

// Defaults to zero for missing keys.
int IntMap_Get(const IntMap *map, int key)
{
    int value;
    IntMap_Lookup(map, key, &value);
    return value;
}

bool IntMap_Lookup(const IntMap *map, int key, int *value)
{
    int v;
    bool found;

    int i = IntMapFindKey(map, key);
    if (i == -1) {
        v = map->myEmptyValue;
        found = FALSE;
    } else {
        ASSERT(MBIntVector_GetValue(&map->myKeys, i) == key);
        v = MBIntVector_GetValue(&map->myValues, i);
    }

    if (value != NULL) {
        *value = v;
    }

    return found;
}

// Returns the new value.
int IntMap_IncrementBy(IntMap *map, int key, int amount)
{
    int *value;
    int i = IntMapGetInsertionIndex(map, key);

    if (i == -1 || !BitVector_Get(&map->myActiveFlags, i)) {
        IntMapPutHelper(map, i, key, map->myEmptyValue);
        i = IntMapGetInsertionIndex(map, key);
    }

    ASSERT(MBIntVector_GetValue(&map->myKeys, i) == key);
    value = MBIntVector_GetPtr(&map->myValues, i);
    *value += amount;
    return *value;
}

void IntMap_Put(IntMap *map, int key, int value)
{
    int i = IntMapGetInsertionIndex(map, key);
    IntMapPutHelper(map, i, key, value);
}

/*
 * Returns true iff the map changed.
 *
 * Note that removing an entry of (1, DEFAULT) will "change" the map,
 * even though Get(1) will return DEFAULT before and after the Remove call, due
 * to the default value of DEFAULT.
 */
bool IntMap_Remove(IntMap *map, int key)
{
    int i = IntMapFindKey(map, key);
    if (i == -1) {
        return FALSE;
    }

    ASSERT(BitVector_Get(&map->myActiveFlags, i));
    ASSERT(BitVector_Get(&map->myFullFlags, i));
    ASSERT(MBIntVector_GetValue(&map->myKeys, i) == key);

    BitVector_Reset(&map->myActiveFlags, i);
    map->mySize--;
    map->myFreeSpace++;
    return TRUE;
}

void IntMap_InsertAll(IntMap *dest, const IntMap *src)
{
    int i = 0;
    int count = 0;

    while (count < src->mySize) {
        ASSERT(i < src->mySpace);

        if (BitVector_Get(&src->myActiveFlags, i)) {
            IntMap_Put(dest, MBIntVector_GetValue(&src->myKeys, i),
                       MBIntVector_GetValue(&src->myValues, i));
            count++;
        }
        i++;
    }
}

static int IntMapHash(const IntMap *map, int key)
{
    uint32 hash;
    uint32 mix;

    // This seems to help my benchmarking tests
    // though I'm not sure if it's a real improvement
    // to the hashing algorithm, or just a quirk
    // of the particular workload I'm running.
    mix = 0x45678;
    mix = key ^ (mix);
    hash = mix % map->mySpace;

    ASSERT(hash >= 0);
    ASSERT(map->mySpace > 0);
    ASSERT(hash < (uint32) map->mySpace);

    return hash;
}

//Returns the index of a valid key, or -1.
static int IntMapFindKey(const IntMap *map, int key)
{
    int hashI = IntMapHash(map, key);
    int x = hashI;

    do {
        if (!BitVector_Get(&map->myFullFlags, x)) {
            return -1;
        } else if (MBIntVector_GetValue(&map->myKeys, x) == key &&
                   BitVector_Get(&map->myActiveFlags, x)) {
            return x;
        }

        x += SEARCH_INCR;
        x %= map->mySpace;
    } while (x != hashI);

    //We've been through the whole table.
    return -1;
}

/*
 * Return the index of where this key would be inserted
 * or -1 if the map is full.
 * In other words, if the key is in the map, return it,
 * and if not, return the next free index.
 */
static int IntMapGetInsertionIndex(const IntMap *map, int key)
{
    int hashI = IntMapHash(map, key);
    int x = hashI;

    ASSERT(hashI % map->mySpace == hashI);

    do {
        if (!BitVector_Get(&map->myFullFlags, x)) {
            return x;
        } else if (MBIntVector_GetValue(&map->myKeys, x) == key) {
            return x;
        }

        x += SEARCH_INCR;
        x %= map->mySpace;

    } while (x != hashI);

    //We've been through the whole table without finding a free spot.
    return -1;
}

//Puts a key at the specified index.
static void IntMapPutHelper(IntMap *map, int index, int key, int value)
{
    ASSERT(index == -1 || index >= 0);
    if (index == -1) {
        ASSERT(map->myFreeSpace == 0 || map->myFullSpace == map->mySpace);
    }

    if (index == -1 || ((double)map->myFullSpace+1)/map->mySpace >= map->myLoad) {
        IntMapRehash(map);
        ASSERT(map->mySpace % SEARCH_INCR == 1);

        index = IntMapGetInsertionIndex(map, key);
    }

    //we are guaranteed to have an empty spot
    ASSERT(map->mySize < map->mySpace);
    ASSERT(map->myFreeSpace > 0);
    ASSERT(map->mySpace % SEARCH_INCR == 1);

    ASSERT(index >= 0);
    ASSERT(index < map->mySpace);
    ASSERT(!BitVector_Get(&map->myActiveFlags, index) ||
           MBIntVector_GetValue(&map->myKeys, index) == key);

    if (!BitVector_Get(&map->myActiveFlags, index)) {
        map->mySize++;

        if (!BitVector_Get(&map->myFullFlags, index)) {
            map->myFreeSpace--;
            map->myFullSpace++;
            BitVector_Set(&map->myFullFlags, index);
        }

        BitVector_Set(&map->myActiveFlags, index);
        MBIntVector_PutValue(&map->myKeys, index, key);
    } else {
        ASSERT(MBIntVector_GetValue(&map->myKeys, index) == key);
        ASSERT(BitVector_Get(&map->myFullFlags, index));
        ASSERT(BitVector_Get(&map->myActiveFlags, index));
    }

    ASSERT(MBIntVector_GetValue(&map->myKeys, index) == key);
    ASSERT(BitVector_Get(&map->myFullFlags, index));
    ASSERT(BitVector_Get(&map->myActiveFlags, index));

    MBIntVector_PutValue(&map->myValues, index, value);
}

void IntMap_DebugDump(IntMap *map)
{
    uint32 i;
    DebugPrint("mySize=%d, mySpace=%d, myFreeSpace=%d\n",
               map->mySize, map->mySpace, map->myFreeSpace);
    for (i = 0; i < MBIntVector_Size(&map->myKeys); i++) {
        if (BitVector_Get(&map->myActiveFlags, i)) {
            DebugPrint("myKeys[%d]=%d, myValues[%d]=%d\n",
                       i, MBIntVector_GetValue(&map->myKeys, i),
                       i, MBIntVector_GetValue(&map->myValues, i));
        } else if (BitVector_Get(&map->myFullFlags, i)) {
            DebugPrint("myKeys[%d]=full\n", i);
        }
    }
}

//Make the underlying table larger
static void IntMapRehash(IntMap *map)
{
    int newSpace = map->mySpace*2 + 1;
    BitVector oldFull;
    BitVector oldActive;
    MBIntVector oldKeys;
    MBIntVector oldValues;
    int x;

    BitVector_Create(&oldFull);
    BitVector_Create(&oldActive);
    MBIntVector_CreateEmpty(&oldKeys);
    MBIntVector_CreateEmpty(&oldValues);

    while (map->mySize/((double)newSpace) > map->myLoad) {
        newSpace = newSpace*2 + 1;
    }

    ASSERT(newSpace > map->mySpace);
    ASSERT(newSpace % SEARCH_INCR == 1);

    BitVector_Consume(&oldFull, &map->myFullFlags);
    BitVector_Consume(&oldActive, &map->myActiveFlags);
    MBIntVector_Consume(&oldKeys, &map->myKeys);
    MBIntVector_Consume(&oldValues, &map->myValues);

    //the flags should all be false
    BitVector_Resize(&map->myFullFlags, newSpace);
    BitVector_Resize(&map->myActiveFlags, newSpace);
    MBIntVector_Resize(&map->myKeys, newSpace);
    MBIntVector_Resize(&map->myValues, newSpace);

    map->mySpace = newSpace;
    map->mySize = 0;
    map->myFullSpace = 0;
    map->myFreeSpace = map->mySpace;

    for (x = 0; x < MBIntVector_Size(&oldKeys); x++) {
        if (BitVector_Get(&oldFull,x) && BitVector_Get(&oldActive, x)) {
            IntMap_Put(map, MBIntVector_GetValue(&oldKeys, x),
                       MBIntVector_GetValue(&oldValues, x));
        }
    }

    ASSERT(map->mySpace % SEARCH_INCR == 1);

    BitVector_Destroy(&oldFull);
    BitVector_Destroy(&oldActive);
    MBIntVector_Destroy(&oldKeys);
    MBIntVector_Destroy(&oldValues);
}
