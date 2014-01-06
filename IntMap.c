/*
 * IntMap.c --
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
    map->myLoad = DEFAULT_LOAD;
}

void IntMap_Destroy(IntMap *map)
{
    MBIntVector_Destroy(&map->myKeys);
    MBIntVector_Destroy(&map->myValues);
    BitVector_Destroy(&map->myActiveFlags);
    BitVector_Destroy(&map->myFullFlags);
}

void IntMap_MakeEmpty(IntMap *map)
{
    BitVector_ResetAll(&map->myFullFlags);
    BitVector_ResetAll(&map->myActiveFlags);
    map->mySize = 0;
    map->myFreeSpace = map->mySpace;
}

bool IntMap_ContainKey(const IntMap *map, int key)
{
    return IntMapFindKey(map, key) != -1;
}

bool IntMap_IsEmpty(const IntMap *map)
{
    return map->mySize == 0;
}

int IntMap_Size(const IntMap *map)
{
    return map->mySize;
}

// Defaults to zero for missing keys.
int IntMap_Get(const IntMap *map, int key)
{
    int i = IntMapFindKey(map, key);
    if (i == -1) {
        return 0;
    }

    return MBIntVector_GetValue(&map->myValues, i);
}

// Returns the new value.
int IntMap_Increment(IntMap *map, int key)
{
    return IntMap_IncrementBy(map, key, 1);
}

// Returns the new value.
int IntMap_Decrement(IntMap *map, int key)
{
    return IntMap_DecrementBy(map, key, 1);
}

// Returns the new value.
int IntMap_IncrementBy(IntMap *map, int key, int amount)
{
    int *value;
    int i = IntMapGetInsertionIndex(map, key);

    if (i == -1 || !BitVector_Get(&map->myActiveFlags, i)) {
        IntMapPutHelper(map, i, key, amount);
    }

    ASSERT(MBIntVector_GetValue(&map->myKeys, i) == key);
    value = MBIntVector_GetPtr(&map->myValues, i);
    *value += amount;
    return *value;
}

// Returns the new value.
int IntMap_DecrementBy(IntMap *map, int key, int amount)
{
    return IntMap_IncrementBy(map, key, -amount);
}

void IntMap_Put(IntMap *map, int key, int value)
{
    int i = IntMapGetInsertionIndex(map, key);
    IntMapPutHelper(map, i, key, value);
}

/*
 * Returns true iff the map changed.
 *
 * Note that removing an entry of (1, 0) will "change" the map,
 * even though Get(1) will return 0 before and after the Remove call, due
 * to the default value of zero.
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

static int IntMapGetInsertionIndex(const IntMap *map, int key)
{
    int hashI = IntMapHash(map, key);
    int x = hashI;

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


static void IntMapPutHelper(IntMap *map, int index, int key, int value)
{
    ASSERT(IMPLIES(index == -1, map->myFreeSpace == 0));
    if (index == -1 || ((double)map->mySize+1)/map->mySpace >= map->myLoad) {
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
