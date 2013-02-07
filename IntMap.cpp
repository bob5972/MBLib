#include "IntMap.hpp"

#include "mbassert.h"

#define DEFAULT_SPACE 15
#define DEFAULT_LOAD  (0.60)
#define SEARCH_INCR   2

IntMap::IntMap()
:myKeys(DEFAULT_SPACE),
 myValues(DEFAULT_SPACE),
 myActiveFlags(DEFAULT_SPACE,FALSE),
 myFullFlags(DEFAULT_SPACE,FALSE),
 mySize(0),
 mySpace(DEFAULT_SPACE),
 myFreeSpace(DEFAULT_SPACE),
 myLoad(DEFAULT_LOAD)
{ }


IntMap::IntMap(int space)
:myKeys(0),
 myValues(0),
 myActiveFlags(0,FALSE),
 myFullFlags(0,FALSE),
 mySize(0),
 mySpace(0),
 myFreeSpace(0),
 myLoad(DEFAULT_LOAD)
{
    ASSERT(space >= 0);

    if (space < DEFAULT_SPACE) {
        space = DEFAULT_SPACE;
    }

    myKeys.resize(space);
    myValues.resize(space);
    myFullFlags.resize(space);
    myActiveFlags.resize(space);
    mySize = 0;
    mySpace = space;
    myFreeSpace = space;
}

IntMap::IntMap(int space, double load)
:myKeys(0),
 myValues(0),
 myActiveFlags(0,FALSE),
 myFullFlags(0,FALSE),
 mySize(0),
 mySpace(0),
 myFreeSpace(0),
 myLoad(load)
{
    ASSERT(space >= 0);
    ASSERT(load > 0);
    ASSERT(load <= 1.0);

    if (space < DEFAULT_SPACE) {
        space = DEFAULT_SPACE;
    }

    myKeys.resize(space);
    myValues.resize(space);
    myFullFlags.resize(space);
    myActiveFlags.resize(space);
    mySize = 0;
    mySpace = space;
    myFreeSpace = space;

    ASSERT(mySpace % SEARCH_INCR == 1);
}

IntMap::IntMap(const IntMap &m)
:myKeys(0),
 myValues(0),
 myActiveFlags(0,FALSE),
 myFullFlags(0,FALSE),
 mySize(0),
 mySpace(0),
 myFreeSpace(0),
 myLoad(DEFAULT_LOAD)
{
    int space = m.size()/myLoad+1;

    if (space < DEFAULT_SPACE) {
        space = DEFAULT_SPACE;
    }

    if (space % SEARCH_INCR != 1) {
        space = space - (space%SEARCH_INCR)+1;
    }

    myKeys.resize(space);
    myValues.resize(space);
    myFullFlags.resize(space);
    myActiveFlags.resize(space);
    mySize = 0;
    mySpace = space;
    myFreeSpace = space;

    for (int x=0;x<m.myKeys.size();x++) {
        if (m.myActiveFlags.get(x)) {
            put(m.myKeys[x],m.myValues[x]);
        }
    }

    ASSERT(mySpace % SEARCH_INCR == 1);
}


int IntMap::findKey(int key) const
{
    int hashI = hash(key);
    int x=hashI;
    bool firstTime = true;

    while (x != hashI || firstTime) {
        if (!myFullFlags.get(x)) {
            return -1;
        } else if (myKeys[x] == key && myActiveFlags.get(x)) {
            return x;
        }

        x += SEARCH_INCR;
        x %= mySpace;
        firstTime = false;
    }

    //we've been through the whole table
    return -1;
}

int IntMap::getInsertionIndex(int key) const
{
    int hashI = hash(key);
    int x=hashI;
    bool firstTime = true;

    while (x != hashI || firstTime) {
        if (!myFullFlags.get(x)) {
            return x;
        } else if (myKeys[x] == key) {
            return x;
        }

        x += SEARCH_INCR;
        x %= mySpace;
        firstTime=false;
    }

    //we've been through the whole table without finding a free spot
    return -1;
}

int IntMap::increment(int key, int amount)
{
    int i = getInsertionIndex(key);
    if (i == -1 || !myActiveFlags.get(i)) {
        putHelper(i, key, amount);
        return amount;
    }

    ASSERT(myKeys[i] == key);
    myValues[i] += amount;
    return myValues[i];
}

int IntMap::decrement(int key, int amount)
{
    int i = getInsertionIndex(key);
    if (i == -1 || !myActiveFlags.get(i)) {
        putHelper(i, key, -amount);
        return -amount;
    }

    ASSERT(myKeys[i] == key);
    myValues[i] -= amount;
    return myValues[i];
}

void IntMap::put(int key, int value)
{
    int ind = getInsertionIndex(key);

    putHelper(ind, key, value);
}

void IntMap::putHelper(int index, int key, int value)
{
    ASSERT(IMPLIES(index == -1, myFreeSpace == 0));
    if (index == -1 || ((double)mySize+1)/mySpace >= myLoad) {
        rehash();
        ASSERT(mySpace % SEARCH_INCR == 1);

        index = getInsertionIndex(key);
    }

    //we are guaranteed to have an empty spot
    ASSERT(mySize < mySpace);
    ASSERT(myFreeSpace > 0);
    ASSERT(mySpace % SEARCH_INCR == 1);

    ASSERT(index >= 0);
    ASSERT(index < mySpace);
    ASSERT(!myActiveFlags.get(index) ||
           myKeys[index] == key);

    if (!myActiveFlags.get(index)) {
        mySize++;

        if (!myFullFlags.get(index)) {
            myFreeSpace--;
            myFullFlags.set(index);
        }

        myActiveFlags.set(index);
        myKeys[index] = key;
    } else {
        ASSERT(myKeys[index] == key);
        ASSERT(myFullFlags.get(index));
        ASSERT(myActiveFlags.get(index));
    }

    ASSERT(myKeys[index] == key);
    ASSERT(myFullFlags.get(index));
    ASSERT(myActiveFlags.get(index));

    myValues[index] = value;
}

void IntMap::rehash()
{
    int newSpace = mySpace*2 + 1;

    while (mySize/((double)newSpace) > myLoad) {
        newSpace = newSpace*2 + 1;
    }

    ASSERT(newSpace > mySpace);
    ASSERT(newSpace % SEARCH_INCR == 1);

    BitVector oldFull;
    BitVector oldActive;
    MBVector<int> oldKeys;
    MBVector<int> oldValues;

    oldFull.consume(myFullFlags);
    oldActive.consume(myActiveFlags);
    oldKeys.consume(myKeys);
    oldValues.consume(myValues);

    //the flags should all be false
    myFullFlags.resize(newSpace);
    myActiveFlags.resize(newSpace);
    myKeys.resize(newSpace);
    myValues.resize(newSpace);

    mySpace = newSpace;
    mySize = 0;
    myFreeSpace = mySpace;

    for (int x=0;x<oldKeys.size();x++) {
        if (oldFull.get(x) && oldActive.get(x)) {
            put(oldKeys[x], oldValues[x]);
        }
    }

    ASSERT(mySpace % SEARCH_INCR == 1);
}


bool IntMap::remove(int key)
{
    int ind = findKey(key);
    if (ind == -1) {
        return false;
    }

    ASSERT(myActiveFlags.get(ind));
    ASSERT(myFullFlags.get(ind));

    //otherwise, we gotta remove it
    ASSERT(myKeys[ind] == key);

    myActiveFlags.reset(ind);
    mySize--;
    myFreeSpace++;
    return true;
}


void IntMap::insertAll(const IntMap& m)
{
    int x=0;
    int count = 0;
    while (count < m.mySize) {
        ASSERT(x < m.mySpace);

        if (m.myActiveFlags.get(x)) {
            put(m.myKeys[x], m.myValues[x]);
            count++;
        }
        x++;
    }
}

void IntMap::makeEmpty()
{
    myFullFlags.resetAll();
    myActiveFlags.resetAll();
    mySize = 0;
    myFreeSpace = mySpace;
}


