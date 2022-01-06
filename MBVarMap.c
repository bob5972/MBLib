/*
 * MBVarMap.c -- part of MBLib
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

#include "MBVarMap.h"

#define DEFAULT_SPACE 16
#define DEFAULT_LOAD  (0.66f)
#define SEARCH_INCR   3

static int CMBVarMapFindHelper(const CMBVarMap *map, MBVar key, bool useInsertion);
static int CMBVarMapFindKey(const CMBVarMap *map, MBVar key);
static int CMBVarMapGetInsertionIndex(const CMBVarMap *map, MBVar key);
static int CMBVarMapHash(const CMBVarMap *map, MBVar key);
static void CMBVarMapPutHelper(CMBVarMap *map, int index, MBVar key, MBVar value);
static void CMBVarMapRehash(CMBVarMap *map);


void CMBVarMap_Create(CMBVarMap *map)
{
    CMBVarVec_CreateWithSize(&map->myKeys, DEFAULT_SPACE);
    CMBVarVec_CreateWithSize(&map->myValues, DEFAULT_SPACE);

    BitVector_CreateWithSize(&map->myFlags, DEFAULT_SPACE * 2);
    ASSERT(BitVector_GetFillValue(&map->myFlags) == FALSE);

    map->mySize = 0;
    map->mySpace = DEFAULT_SPACE;
    map->myFreeSpace = DEFAULT_SPACE;
    map->myFullSpace = 0;
    map->myTargetLoad = DEFAULT_LOAD * map->mySpace;

    ASSERT(MBUtil_IsPow2(map->mySpace));
    map->myIndexMask = map->mySpace - 1;

    // Zero is the default emptyValue.
    map->myEmptyValue.vUint64 = 0;
}

void CMBVarMap_Destroy(CMBVarMap *map)
{
    CMBVarVec_Destroy(&map->myKeys);
    CMBVarVec_Destroy(&map->myValues);
    BitVector_Destroy(&map->myFlags);
}

void CMBVarMap_SetEmptyValue(CMBVarMap *map, MBVar emptyValue)
{
    map->myEmptyValue = emptyValue;
}

void CMBVarMap_MakeEmpty(CMBVarMap *map)
{
    BitVector_ResetAll(&map->myFlags);
    map->mySize = 0;
    map->myFullSpace = 0;
    map->myFreeSpace = map->mySpace;
}

bool CMBVarMap_ContainsKey(const CMBVarMap *map, MBVar key)
{
    return CMBVarMapFindKey(map, key) != -1;
}

// Defaults to zero for missing keys.
MBVar CMBVarMap_Get(const CMBVarMap *map, MBVar key)
{
    MBVar value;
    CMBVarMap_Lookup(map, key, &value);
    return value;
}

bool CMBVarMap_Lookup(const CMBVarMap *map, MBVar key, MBVar *value)
{
    MBVar v;
    bool found;

    int i = CMBVarMapFindKey(map, key);
    if (i == -1) {
        v = map->myEmptyValue;
        found = FALSE;
    } else {
        ASSERT(CMBVarVec_GetValue(&map->myKeys, i).all == key.all);
        v = CMBVarVec_GetValue(&map->myValues, i);
        found = TRUE;
    }

    if (value != NULL) {
        *value = v;
    }

    return found;
}

// Returns the new value.
int CMBVarMap_IncrementByInt32(CMBVarMap *map, MBVar key, int amount)
{
    MBVar *value;
    int i = CMBVarMapGetInsertionIndex(map, key);

    if (i == -1 || !CMBVarMapGetActiveFlag(map, i)) {
        CMBVarMapPutHelper(map, i, key, map->myEmptyValue);
        i = CMBVarMapGetInsertionIndex(map, key);
    }

    ASSERT(CMBVarVec_GetValue(&map->myKeys, i).all == key.all);
    value = CMBVarVec_GetPtr(&map->myValues, i);
    value->vInt32 += amount;
    return value->vInt32;
}

void CMBVarMap_Put(CMBVarMap *map, MBVar key, MBVar value)
{
    int i = CMBVarMapGetInsertionIndex(map, key);
    CMBVarMapPutHelper(map, i, key, value);
}

/*
 * Returns true iff the map changed.
 *
 * Note that removing an entry of (1, DEFAULT) will "change" the map,
 * even though Get(1) will return DEFAULT before and after the Remove call, due
 * to the default value of DEFAULT.
 */
bool CMBVarMap_Remove(CMBVarMap *map, MBVar key)
{
    int i = CMBVarMapFindKey(map, key);
    if (i == -1) {
        return FALSE;
    }

    ASSERT(CMBVarMapGetActiveFlag(map, i));
    ASSERT(CMBVarMapGetFullFlag(map, i));
    ASSERT(CMBVarVec_GetValue(&map->myKeys, i).all == key.all);

    CMBVarMapResetActiveFlag(map, i);
    map->mySize--;
    map->myFreeSpace++;
    return TRUE;
}

void CMBVarMap_InsertAll(CMBVarMap *dest, const CMBVarMap *src)
{
    int i = 0;
    int count = 0;

    while (count < src->mySize) {
        ASSERT(i < src->mySpace);

        if (CMBVarMapGetActiveFlag(src, i)) {
            CMBVarMap_Put(dest, CMBVarVec_GetValue(&src->myKeys, i),
                       CMBVarVec_GetValue(&src->myValues, i));
            count++;
        }
        i++;
    }
}

static int CMBVarMapHash(const CMBVarMap *map, MBVar key)
{
    uint64 ukey = key.all;
    uint64 mix1 = 0x87654321F7345678;
    uint64 rotKey;
    uint64 hash;

    rotKey = ((ukey & 0xFF00FF00FF00FF00) >> 8) +
             (ukey & 0x00FF00FF00FF00FF);
    rotKey = (rotKey >> 16) + rotKey;
    rotKey = (rotKey >> 32) + rotKey;

    hash = ukey + rotKey + ((ukey >> 19) ^ mix1);

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
static int CMBVarMapFindHelper(const CMBVarMap *map, MBVar key, bool useInsertion)
{
    int hashI = CMBVarMapHash(map, key);
    int x = hashI;
    int i;
    int mySpace = map->mySpace;

    for (i = 0; i < mySpace; i++) {
        ASSERT(x < map->mySpace);
        if (!CMBVarMapGetFullFlag(map, x)) {
            return useInsertion ? x : -1;
        } else if (CMBVarVec_GetValue(&map->myKeys, x).all == key.all &&
                   (useInsertion || CMBVarMapGetActiveFlag(map, x))) {
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
static int CMBVarMapFindKey(const CMBVarMap *map, MBVar key)
{
   return CMBVarMapFindHelper(map, key, FALSE);
}

/*
 * Return the index of where this key would be inserted
 * or -1 if the map is full.
 * In other words, if the key is in the map, return it,
 * and if not, return the next free index.
 */
static int CMBVarMapGetInsertionIndex(const CMBVarMap *map, MBVar key)
{
   return CMBVarMapFindHelper(map, key, TRUE);
}

//Puts a key at the specified index.
static void CMBVarMapPutHelper(CMBVarMap *map, int index, MBVar key, MBVar value)
{
    ASSERT(index == -1 || index >= 0);
    if (index == -1) {
        ASSERT(map->myFreeSpace == 0 || map->myFullSpace == map->mySpace);
    }

    if (index == -1 || map->myFullSpace + 1 >= map->myTargetLoad) {
        CMBVarMapRehash(map);

        index = CMBVarMapGetInsertionIndex(map, key);
    }

    //we are guaranteed to have an empty spot
    ASSERT(map->mySize < map->mySpace);
    ASSERT(map->myFreeSpace > 0);

    ASSERT(index >= 0);
    ASSERT(index < map->mySpace);
    ASSERT(!CMBVarMapGetActiveFlag(map, index) ||
           CMBVarVec_GetValue(&map->myKeys, index).all == key.all);

    if (!CMBVarMapGetActiveFlag(map, index)) {
        map->mySize++;

        if (!CMBVarMapGetFullFlag(map, index)) {
            map->myFreeSpace--;
            map->myFullSpace++;
            CMBVarMapSetFullFlag(map, index);
        }

        CMBVarMapSetActiveFlag(map, index);
        CMBVarVec_PutValue(&map->myKeys, index, key);
    } else {
        ASSERT(CMBVarVec_GetValue(&map->myKeys, index).all == key.all);
        ASSERT(CMBVarMapGetFullFlag(map, index));
        ASSERT(CMBVarMapGetActiveFlag(map, index));
    }

    ASSERT(CMBVarVec_GetValue(&map->myKeys, index).all == key.all);
    ASSERT(CMBVarMapGetFullFlag(map, index));
    ASSERT(CMBVarMapGetActiveFlag(map, index));

    CMBVarVec_PutValue(&map->myValues, index, value);
}

void CMBVarMap_DebugDump(const CMBVarMap *map)
{
    uint32 i;
    DebugPrint("mySize=%d, mySpace=%d, myFreeSpace=%d\n",
               map->mySize, map->mySpace, map->myFreeSpace);
    for (i = 0; i < CMBVarVec_Size(&map->myKeys); i++) {
        if (CMBVarMapGetActiveFlag(map, i)) {
            DebugPrint("myKeys[%d]=%d, myValues[%d]=%d\n",
                       i, CMBVarVec_GetValue(&map->myKeys, i),
                       i, CMBVarVec_GetValue(&map->myValues, i));
        } else if (CMBVarMapGetFullFlag(map, i)) {
            DebugPrint("myKeys[%d]=full\n", i);
        }
    }
}

//Make the underlying table larger
static void CMBVarMapRehash(CMBVarMap *map)
{
    int newSpace;
    BitVector oldFlags;
    CMBVarVec oldKeys;
    CMBVarVec oldValues;
    int x;

    BitVector_Create(&oldFlags);
    CMBVarVec_CreateEmpty(&oldKeys);
    CMBVarVec_CreateEmpty(&oldValues);

    newSpace = map->mySpace;
    while (map->mySize / ((float)newSpace) > DEFAULT_LOAD) {
        newSpace *= 2;
    }
    ASSERT(newSpace > 0);
    ASSERT(MBUtil_IsPow2(newSpace));
    ASSERT(newSpace >= map->mySpace);

    BitVector_Consume(&oldFlags, &map->myFlags);
    CMBVarVec_Consume(&oldKeys, &map->myKeys);
    CMBVarVec_Consume(&oldValues, &map->myValues);

    //the flags should all be false
    BitVector_Resize(&map->myFlags, newSpace * 2);
    CMBVarVec_Resize(&map->myKeys, newSpace);
    CMBVarVec_Resize(&map->myValues, newSpace);

    map->mySpace = newSpace;
    map->mySize = 0;
    map->myFullSpace = 0;
    map->myFreeSpace = map->mySpace;
    map->myTargetLoad = map->mySpace * DEFAULT_LOAD;
    map->myIndexMask = map->mySpace - 1;

    for (x = 0; x < CMBVarVec_Size(&oldKeys); x++) {
        bool oldFull = BitVector_Get(&oldFlags, x * 2 + 0);
        bool oldActive = BitVector_Get(&oldFlags, x * 2 + 1);
        if (oldFull && oldActive) {
            CMBVarMap_Put(map, CMBVarVec_GetValue(&oldKeys, x),
                        CMBVarVec_GetValue(&oldValues, x));
        }
    }

    BitVector_Destroy(&oldFlags);
    CMBVarVec_Destroy(&oldKeys);
    CMBVarVec_Destroy(&oldValues);
}
