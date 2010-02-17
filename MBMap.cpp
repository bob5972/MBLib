#ifndef MBMap_CPP_201002111716
#define MBMap_CPP_201002111716


#include "MBMap.h"
#include "mjbassert.h"

template<class keyType, class valueType>
MBMap<keyType,valueType>::MBMap()
:myMap()
{  }

template<class keyType, class valueType>
MBMap<keyType,valueType>::~MBMap()
{   }


template<class keyType, class valueType>
void MBMap<keyType,valueType>::clear()
{
	makeEmpty();
}

template<class keyType, class valueType>
void MBMap<keyType,valueType>::makeEmpty()
{
	myMap.clear();
}

template<class keyType, class valueType>
bool MBMap<keyType,valueType>::isEmpty() const
{
	return size() == 0;
}

template<class keyType, class valueType>
int MBMap<keyType,valueType>::size() const
{
	return myMap.size();
}

template<class keyType, class valueType>
bool MBMap<keyType,valueType>::containsKey(const keyType&key) const
{
	return myMap.count(key) > 0;
}

template<class keyType, class valueType>
bool MBMap<keyType,valueType>::remove(const keyType& key)
{
	return myMap.erase(key) > 0;
}

template<class keyType, class valueType>
const valueType& MBMap<keyType,valueType>::get(const keyType& key) const
{
	//There's probably a better way that'll do this in one query
	
	if(myMap.find(key) != myMap.end()) {
		PANIC("Map element not found!");
	}
	
	return myMap[key];
}

template<class keyType, class valueType>
valueType& MBMap<keyType,valueType>::get(const keyType& key)
{
	//There's probably a better way that'll do this in one query
	
	if(myMap.find(key) == myMap.end()) {
		PANIC("Map element not found!");
	}
	
	return myMap[key];
}


template<class keyType, class valueType>
bool MBMap<keyType,valueType>::put(const keyType& key, const valueType& val)
{
	bool oup = containsKey(key);
	
	myMap[key] = val;
	return oup;
}

template<class keyType, class valueType>
valueType& MBMap<keyType,valueType>::operator [] (const keyType & key)
{
	return myMap[key];
}

template<class keyType, class valueType>
const valueType& MBMap<keyType,valueType>::operator [] (const keyType & key) const
{
	return myMap[key];
}


#endif //MBMap_CPP_201002111716
