#include "IntMap.h"

#include "mbassert.h"

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
	
	if(space < DEFAULT_SPACE) {
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
	
	if(space < DEFAULT_SPACE) {
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
	
	if(space < DEFAULT_SPACE) {
		space = DEFAULT_SPACE;
	}
	
	if(space % SEARCH_INCR != 1) {
		space = space - (space%SEARCH_INCR)+1;
	}
	
	myKeys.resize(space);
	myValues.resize(space);
	myFullFlags.resize(space);
	myActiveFlags.resize(space);
	mySize = 0;
	mySpace = space;
	myFreeSpace = space;
	
	for(int x=0;x<m.myKeys.size();x++) {
		if(m.myActiveFlags.get(x)) {
			put(m.myKeys[x],m.myValues[x]);
		}
	}
	
	ASSERT(mySpace % SEARCH_INCR == 1);
}

void IntMap::makeEmpty()
{
	myFullFlags.resetAll();
	myActiveFlags.resetAll();
	mySize = 0;
	myFreeSpace = mySpace;
}

bool IntMap::containsKey(int key) const
{
	return findKey(key) != -1;
}

int IntMap::hash(int key) const
{
	int hash = key%mySpace;
	if(hash < 0) {
		hash = -hash;
	}
	
	return hash;
}

int IntMap::findKey(int key) const
{
	int hashI = hash(key);	
	int x=hashI;
	bool firstTime = true;
	
	while(x != hashI || firstTime) {
		if(!myFullFlags.get(x)) {
			return -1;
		} else if (myKeys[x] == key && myActiveFlags.get(x)) {
			return x;
		}
		
		x+= SEARCH_INCR;
		x%= mySpace;
		firstTime = false;
	}
	
	//we've been through the whole table
	return -1;
}

int IntMap::getIndexOfKey(int key) const
{
	int hashI = hash(key);		
	int x=hashI;
	bool firstTime = true;

	while(x != hashI || firstTime) {
		if(!myFullFlags.get(x)) {
			return x;
		} else if(myKeys[x] == key) {
			return x;
		}
		
		x+=SEARCH_INCR;
		x%=mySpace;
		firstTime=false;
	}
	
	//we've been through the whole table without finding a free spot
	return -1;
}

int IntMap::getFreeIndex(int key) const
{
	ASSERT(mySpace % SEARCH_INCR == 1);
	
	int hashI = hash(key);		
	int x=hashI;
	bool firstTime = true;

	while(x != hashI || firstTime) {
		if(!myActiveFlags.get(x)) {
			return x;
		}
		
		x+=SEARCH_INCR;
		x%=mySpace;
		firstTime=false;
	}
	
	//we've been through the whole table without finding a free spot
	return -1;
}


int IntMap::get(int key) const
{
	int i = findKey(key);
	
	if( i == -1) {
		return 0;
	}
	
	return myValues[i];
}

int IntMap::increment(int key)
{
	return increment(key, 1);
}

int IntMap::increment(int key, int amount)
{
	int i = findKey(key);
	if(i == -1) {
		put(key,amount);
		return amount;
	}
	
	myValues[i] += amount;
	return myValues[i];
}

int IntMap::decrement(int key)
{
	return decrement(key,1);
}

int IntMap::decrement(int key, int amount)
{
	int i = findKey(key);
	if(i == -1) {
		put(key,-amount);
		return -amount;
	}
	
	myValues[i] -= amount;
	return myValues[i];
}

bool IntMap::isEmpty() const
{
	return mySize == 0;
}

bool IntMap::put(int key, int value)
{
	//load check
	if( mySize + 1 >= mySpace || ((double)mySize+1)/mySpace >= myLoad) {
		rehash();
		ASSERT(mySpace % SEARCH_INCR == 1);
	}
	
	//we are guaranteed to have an empty spot
	ASSERT(mySize < mySpace);
	ASSERT(myFreeSpace > 0);
	ASSERT(mySpace % SEARCH_INCR == 1);
		
	int ind = getIndexOfKey(key);
	
	if(ind != -1) {
		//are we replacing a value?
		if(myFullFlags.get(ind) && myActiveFlags.get(ind)) {
			ASSERT(myKeys[ind] == key);
			
			int oldValue = myValues[ind];
			myValues[ind] = value;
			return (oldValue != value);
		}
	}
	
	//key is NOT in the tree
	ASSERT(mySpace % SEARCH_INCR == 1);
	ind = getFreeIndex(key);
	
	ASSERT(ind >= 0);	
	ASSERT(!myActiveFlags.get(ind));
	
	mySize++;
	myFreeSpace--;
	myFullFlags.set(ind);
	myActiveFlags.set(ind);
	
	myKeys[ind] = key;
	myValues[ind] = value;
	return TRUE;
}

void IntMap::rehash()
{
	int newSpace = mySpace*2 + 1;
	
	while(mySize/((double)newSpace) > myLoad) {
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
	
	for(int x=0;x<oldKeys.size();x++) {
		if(oldFull.get(x) && oldActive.get(x)) {
			put(oldKeys[x],oldValues[x]);
		}
	}
	
	ASSERT(mySpace % SEARCH_INCR == 1);
}


bool IntMap::remove(int key)
{
	int ind = getIndexOfKey(key);
	if(ind == -1) {
		return false;
	}
	
	if(!myActiveFlags.get(ind) || !myFullFlags.get(ind)) {
		return false;
	}
	
	//otherwise, we gotta remove it
	ASSERT(myKeys[ind] == key);
	
	myActiveFlags.reset(ind);
	mySize--;
	myFreeSpace++;
	return true;
}

int IntMap::size() const
{
	return mySize;
}


bool IntMap::insertAll(const IntMap& m)
{
	bool oup = FALSE;
	int x=0;
	int count = 0;
	while(count < m.mySize) {
		ASSERT(x < m.mySpace);
		
		if(m.myActiveFlags.get(x)) {
			oup |= put(m.myKeys[x], m.myValues[x]);
			count++;
		}
		x++;
	}
	
	return oup;
}








