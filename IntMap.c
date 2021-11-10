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

#define DEFAULT_SPACE 16
#define DEFAULT_LOAD  (0.66f)
#define SEARCH_INCR   3

static int CIntMapFindHelper(const CIntMap *map, int key, bool useInsertion);
static int CIntMapFindKey(const CIntMap *map, int key);
static int CIntMapGetInsertionIndex(const CIntMap *map, int key);
static int CIntMapHash(const CIntMap *map, int key);
static void CIntMapPutHelper(CIntMap *map, int index, int key, int value);
static void CIntMapRehash(CIntMap *map);


void CIntMap_Create(CIntMap *map)
{
    CMBIntVec_CreateWithSize(&map->myKeys, DEFAULT_SPACE);
    CMBIntVec_CreateWithSize(&map->myValues, DEFAULT_SPACE);

    BitVector_CreateWithSize(&map->myActiveFlags, DEFAULT_SPACE);
    ASSERT(BitVector_GetFillValue(&map->myActiveFlags) == FALSE);
    BitVector_CreateWithSize(&map->myFullFlags, DEFAULT_SPACE);
    ASSERT(BitVector_GetFillValue(&map->myFullFlags) == FALSE);

    map->mySize = 0;
    map->mySpace = DEFAULT_SPACE;
    map->myFreeSpace = DEFAULT_SPACE;
    map->myFullSpace = 0;
    map->myTargetLoad = DEFAULT_LOAD * map->mySpace;

    ASSERT(MBUtil_IsPow2(map->mySpace));
    map->myIndexMask = map->mySpace - 1;

    // Zero is the default emptyValue.
    map->myEmptyValue = 0;
}

void CIntMap_Destroy(CIntMap *map)
{
    CMBIntVec_Destroy(&map->myKeys);
    CMBIntVec_Destroy(&map->myValues);
    BitVector_Destroy(&map->myActiveFlags);
    BitVector_Destroy(&map->myFullFlags);
}

void CIntMap_SetEmptyValue(CIntMap *map, int emptyValue)
{
    map->myEmptyValue = emptyValue;
}

void CIntMap_MakeEmpty(CIntMap *map)
{
    BitVector_ResetAll(&map->myFullFlags);
    BitVector_ResetAll(&map->myActiveFlags);
    map->mySize = 0;
    map->myFullSpace = 0;
    map->myFreeSpace = map->mySpace;
}

bool CIntMap_ContainsKey(const CIntMap *map, int key)
{
    return CIntMapFindKey(map, key) != -1;
}

// Defaults to zero for missing keys.
int CIntMap_Get(const CIntMap *map, int key)
{
    int value;
    CIntMap_Lookup(map, key, &value);
    return value;
}

bool CIntMap_Lookup(const CIntMap *map, int key, int *value)
{
    int v;
    bool found;

    int i = CIntMapFindKey(map, key);
    if (i == -1) {
        v = map->myEmptyValue;
        found = FALSE;
    } else {
        ASSERT(CMBIntVec_GetValue(&map->myKeys, i) == key);
        v = CMBIntVec_GetValue(&map->myValues, i);
        found = TRUE;
    }

    if (value != NULL) {
        *value = v;
    }

    return found;
}

// Returns the new value.
int CIntMap_IncrementBy(CIntMap *map, int key, int amount)
{
    int *value;
    int i = CIntMapGetInsertionIndex(map, key);

    if (i == -1 || !BitVector_Get(&map->myActiveFlags, i)) {
        CIntMapPutHelper(map, i, key, map->myEmptyValue);
        i = CIntMapGetInsertionIndex(map, key);
    }

    ASSERT(CMBIntVec_GetValue(&map->myKeys, i) == key);
    value = CMBIntVec_GetPtr(&map->myValues, i);
    *value += amount;
    return *value;
}

void CIntMap_Put(CIntMap *map, int key, int value)
{
    int i = CIntMapGetInsertionIndex(map, key);
    CIntMapPutHelper(map, i, key, value);
}

/*
 * Returns true iff the map changed.
 *
 * Note that removing an entry of (1, DEFAULT) will "change" the map,
 * even though Get(1) will return DEFAULT before and after the Remove call, due
 * to the default value of DEFAULT.
 */
bool CIntMap_Remove(CIntMap *map, int key)
{
    int i = CIntMapFindKey(map, key);
    if (i == -1) {
        return FALSE;
    }

    ASSERT(BitVector_Get(&map->myActiveFlags, i));
    ASSERT(BitVector_Get(&map->myFullFlags, i));
    ASSERT(CMBIntVec_GetValue(&map->myKeys, i) == key);

    BitVector_Reset(&map->myActiveFlags, i);
    map->mySize--;
    map->myFreeSpace++;
    return TRUE;
}

void CIntMap_InsertAll(CIntMap *dest, const CIntMap *src)
{
    int i = 0;
    int count = 0;

    while (count < src->mySize) {
        ASSERT(i < src->mySpace);

        if (BitVector_Get(&src->myActiveFlags, i)) {
            CIntMap_Put(dest, CMBIntVec_GetValue(&src->myKeys, i),
                       CMBIntVec_GetValue(&src->myValues, i));
            count++;
        }
        i++;
    }
}

static int CIntMapHash(const CIntMap *map, int key)
{
    uint32 mix1 = 0xF7345678;
    uint32 rotKey = (key >> 19);
    uint32 hash = (key ^ mix1) ^ rotKey;
    hash &= map->myIndexMask;
    ASSERT(hash < map->mySpace);
    return hash;
}

/*
 * If useInsertion is TRUE, return the index of where this key would be
 * inserted or -1 if the map is full.
 * In other words, if the key is in the map, return it,
 * and if not, return the next free index.
 *
 * If useInsertion is FALSE, return the index of valid key, or -1.
 */
static int CIntMapFindHelper(const CIntMap *map, int key, bool useInsertion)
{
    int hashI = CIntMapHash(map, key);
    int x = hashI;
    int i;
    int mySpace = map->mySpace;

    for (i = 0; i < mySpace; i++) {
        ASSERT(x < map->mySpace);
        if (!BitVector_Get(&map->myFullFlags, x)) {
            return useInsertion ? x : -1;
        } else if (CMBIntVec_GetValue(&map->myKeys, x) == key &&
                   (useInsertion || BitVector_Get(&map->myActiveFlags, x))) {
            return x;
        }

        x += SEARCH_INCR;
        x &= map->myIndexMask;
        ASSERT(x < map->mySpace);
    }
    ASSERT(x == hashI);

    //We've been through the whole table.
    return -1;
}

/*
 * Return the index of valid key, or -1.
 */
static int CIntMapFindKey(const CIntMap *map, int key)
{
   return CIntMapFindHelper(map, key, FALSE);
}

/*
 * Return the index of where this key would be inserted
 * or -1 if the map is full.
 * In other words, if the key is in the map, return it,
 * and if not, return the next free index.
 */
static int CIntMapGetInsertionIndex(const CIntMap *map, int key)
{
   return CIntMapFindHelper(map, key, TRUE);
}

//Puts a key at the specified index.
static void CIntMapPutHelper(CIntMap *map, int index, int key, int value)
{
    ASSERT(index == -1 || index >= 0);
    if (index == -1) {
        ASSERT(map->myFreeSpace == 0 || map->myFullSpace == map->mySpace);
    }

    if (index == -1 || map->myFullSpace + 1 >= map->myTargetLoad) {
        CIntMapRehash(map);

        index = CIntMapGetInsertionIndex(map, key);
    }

    //we are guaranteed to have an empty spot
    ASSERT(map->mySize < map->mySpace);
    ASSERT(map->myFreeSpace > 0);

    ASSERT(index >= 0);
    ASSERT(index < map->mySpace);
    ASSERT(!BitVector_Get(&map->myActiveFlags, index) ||
           CMBIntVec_GetValue(&map->myKeys, index) == key);

    if (!BitVector_Get(&map->myActiveFlags, index)) {
        map->mySize++;

        if (!BitVector_Get(&map->myFullFlags, index)) {
            map->myFreeSpace--;
            map->myFullSpace++;
            BitVector_Set(&map->myFullFlags, index);
        }

        BitVector_Set(&map->myActiveFlags, index);
        CMBIntVec_PutValue(&map->myKeys, index, key);
    } else {
        ASSERT(CMBIntVec_GetValue(&map->myKeys, index) == key);
        ASSERT(BitVector_Get(&map->myFullFlags, index));
        ASSERT(BitVector_Get(&map->myActiveFlags, index));
    }

    ASSERT(CMBIntVec_GetValue(&map->myKeys, index) == key);
    ASSERT(BitVector_Get(&map->myFullFlags, index));
    ASSERT(BitVector_Get(&map->myActiveFlags, index));

    CMBIntVec_PutValue(&map->myValues, index, value);
}

void CIntMap_DebugDump(CIntMap *map)
{
    uint32 i;
    DebugPrint("mySize=%d, mySpace=%d, myFreeSpace=%d\n",
               map->mySize, map->mySpace, map->myFreeSpace);
    for (i = 0; i < CMBIntVec_Size(&map->myKeys); i++) {
        if (BitVector_Get(&map->myActiveFlags, i)) {
            DebugPrint("myKeys[%d]=%d, myValues[%d]=%d\n",
                       i, CMBIntVec_GetValue(&map->myKeys, i),
                       i, CMBIntVec_GetValue(&map->myValues, i));
        } else if (BitVector_Get(&map->myFullFlags, i)) {
            DebugPrint("myKeys[%d]=full\n", i);
        }
    }
}

//Make the underlying table larger
static void CIntMapRehash(CIntMap *map)
{
    int newSpace;
    BitVector oldFull;
    BitVector oldActive;
    CMBIntVec oldKeys;
    CMBIntVec oldValues;
    int x;

    BitVector_Create(&oldFull);
    BitVector_Create(&oldActive);
    CMBIntVec_CreateEmpty(&oldKeys);
    CMBIntVec_CreateEmpty(&oldValues);

    newSpace = map->mySpace;
    while (map->mySize / ((float)newSpace) > DEFAULT_LOAD) {
        newSpace *= 2;
    }
    ASSERT(newSpace > 0);
    ASSERT(MBUtil_IsPow2(newSpace));
    ASSERT(newSpace >= map->mySpace);

    BitVector_Consume(&oldFull, &map->myFullFlags);
    BitVector_Consume(&oldActive, &map->myActiveFlags);
    CMBIntVec_Consume(&oldKeys, &map->myKeys);
    CMBIntVec_Consume(&oldValues, &map->myValues);

    //the flags should all be false
    BitVector_Resize(&map->myFullFlags, newSpace);
    BitVector_Resize(&map->myActiveFlags, newSpace);
    CMBIntVec_Resize(&map->myKeys, newSpace);
    CMBIntVec_Resize(&map->myValues, newSpace);

    map->mySpace = newSpace;
    map->mySize = 0;
    map->myFullSpace = 0;
    map->myFreeSpace = map->mySpace;
    map->myTargetLoad = map->mySpace * DEFAULT_LOAD;
    map->myIndexMask = map->mySpace - 1;

    for (x = 0; x < CMBIntVec_Size(&oldKeys); x++) {
        if (BitVector_Get(&oldFull,x) && BitVector_Get(&oldActive, x)) {
            CIntMap_Put(map, CMBIntVec_GetValue(&oldKeys, x),
                        CMBIntVec_GetValue(&oldValues, x));
        }
    }

    BitVector_Destroy(&oldFull);
    BitVector_Destroy(&oldActive);
    CMBIntVec_Destroy(&oldKeys);
    CMBIntVec_Destroy(&oldValues);
}
