#ifndef MBMAP_CPP_201001091450
#define MBMAP_CPP_201001091450

#include "MBMap.h"
#include "mjbdebug.h"
#include "mjbassert.h"

//Constructors/destructor

template <class keyType, class valueType>
MBMap<keyType,valueType>::MBMap() //Default Constructor - Empty Map
:myKeys(2),
 myValues(2),
 mySize(0)
{
}

template <class keyType, class valueType>
MBMap<keyType,valueType>::MBMap(const MBMap &map) //Copy Constructor
:myKeys(map.myKeys),
 myValues(map.myValues),
 mySize(map.mySize)
{
}
	
template <class keyType, class valueType>
MBMap<keyType,valueType>::~MBMap() //Destructor
{
}
		

//Assignment
template <class keyType, class valueType>
const MBMap<keyType,valueType>& MBMap<keyType,valueType>::operator = (const MBMap<keyType,valueType> & rhs)
{
	myKeys = rhs.myKeys;
	mySize = rhs.mySize;
	myValues = rhs.myValues;
    return *this;
}

//Accessors

template <class keyType, class valueType>
void MBMap<keyType,valueType>::clear()
//Empties the Map
{
	mySize = 0;
}

template <class keyType, class valueType>
bool MBMap<keyType,valueType>::isEmpty() const
//Returns true if Empty
{
	return mySize == 0;
}

template <class keyType, class valueType>
int MBMap<keyType,valueType>::size() const
//Returns the size of the map
{
	return mySize;
}

template <class keyType, class valueType>
bool MBMap<keyType,valueType>::containsKey(const keyType &key) const
//Returns true if key is in the map
{
	return (findKey(key) != -1);
}

template <class keyType, class valueType>
int MBMap<keyType,valueType>::findKey(const keyType &key) const
{
	for (int x=0;x<mySize;x++)
	{
		if (myKeys[x] == key)
			return x;
	}
	return -1;
}

template <class keyType, class valueType>
bool MBMap<keyType,valueType>::addKey(const keyType & key, const valueType & val)
//Returns true if key is already in map
//Overrides existing values
{
	int x(findKey(key));
	if (x == -1)
	{
		mySize++;
		int len (myKeys.length());
		if (len < mySize)
		{
			myKeys.resize(len * 2);
			myValues.resize(len *2);
		}
		myKeys[mySize-1] = key;
		myValues[mySize-1] = val;
		return false;
	}
	else
	{
		myValues[x] = val;
		return true;
	}
}


template <class keyType, class valueType>
bool MBMap<keyType,valueType>::removeKey(const keyType&key)
//Removes a key-value pair from the map
//Returns false if the key is not in the map, and true otherwise
{
	int x(findKey(key));
	if (x == -1)
		return false;
	myKeys[x] = myKeys[mySize -1];
	myValues[x] = myValues[mySize-1];
	mySize--;
	return true;
}
	
	
	

template <class keyType, class valueType>
const valueType& MBMap<keyType,valueType>::getValue(const keyType & key) const
//Returns the value for the key
{
	int x=(findKey(key));
	if (x==-1)
	{
		cerr << "Invalid Key: Not on Map"<<endl;
		exit(1);
	}
	return myValues[x];	
}

template <class keyType, class valueType>
const apvector<keyType>& MBMap<keyType,valueType>::keys()
//Returns an apvector containing all the keys in the map
//Note: No Guaranteed order at this time.
{
	if (mySize == 0)
		return apvector<keyType>(0);
	else
	{
		myKeys.resize(mySize);
		myValues.resize(mySize);
		return myKeys;
	}
}


template <class keyType, class valueType>
const apvector<valueType>& MBMap<keyType,valueType>::values()
//Returns an apvector containing all the values in the map
//Matches the order in the vector returned by keys
//ie keys[x] and values[x] are a key-value pair
{
	if (mySize == 0)
		return apvector<valueType>(0);
	else
	{
		myKeys.resize(mySize);
		myValues.resize(mySize);
		return myValues;
	}
	
}

#endif //MBMAP_CPP_201001091450
