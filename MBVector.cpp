#ifndef MBVector_CPP_201002052320
#define MBVector_CPP_201002052320

#include <stdlib.h>
#include "MBVector.h"
#include "mbdebug.h"
#include "mbassert.h"
#include "mbutil.h"

template<class itemType>
MBVector<itemType>::MBVector()
:mySize(0),
 myCapacity(1),
 myItems(new itemType[1])
{
	ASSERT(myCapacity > 0);
}

template<class itemType>
MBVector<itemType>::MBVector(int size)
:mySize(size),
 myCapacity(size),
 myItems(new itemType[size])
{
	mySize = size;
	ASSERT(mySize >= 0);
	
	if (mySize > 0) {
		myCapacity = mySize;
	} else {
		myCapacity = 1;
	}
	
	myItems = new itemType[myCapacity];
	
	ASSERT(myCapacity > 0);
}

template<class itemType>
MBVector<itemType>::MBVector(const MBVector<itemType>& vec)
:mySize(vec.mySize),
 myCapacity(mySize),
 myItems(new itemType[mySize])
{
	for (int x=0;x < vec.mySize;x++)
	{
		myItems[x] = vec.myItems[x];
	}
	
	ASSERT(myCapacity > 0);
}

template<class itemType>
MBVector<itemType>::MBVector(int size, const itemType & fillValue)
:mySize(size),
 myCapacity(size),
 myItems(new itemType[size])
{
	for (int x=0;x<size;x++)
	{
		myItems[x] = fillValue;
	}
	
	ASSERT(myCapacity > 0);
}

template<class itemType>
MBVector<itemType>::~MBVector()
{
	delete [] myItems;
}

template <class itemType>
void MBVector<itemType>::makeEmpty()
{
	resize(0);
	ASSERT(myCapacity > 0);
}

template <class itemType>
void MBVector<itemType>::consume(MBVector<itemType> &v)
{
	makeEmpty();
	
	myItems = v.myItems;
	mySize = v.mySize;
	myCapacity = v.mySize;
	
	v.myItems = new itemType[1];
	v.mySize = 0;
	v.myCapacity = 1;
	
	ASSERT(myCapacity > 0);
	ASSERT(v.myCapacity > 0);
}

template <class itemType>
const MBVector<itemType> &
	MBVector<itemType>::operator = (const MBVector<itemType> & rhs)
{
	if (this != &rhs) {
		resize(rhs.mySize);
		for (int x = 0;x < mySize;x++) {
			myItems[x] = rhs.myItems[x];
		}
	}
	
	ASSERT(myCapacity > 0);
	
	return *this;
}

template<class itemType>
int MBVector<itemType>::find(const itemType & item) const
{
	for(int x=0;x<mySize;x++) {
		if(myItems[x] == item) {
			return x;
		}
	}
	return -1;
}

template <class itemType>
itemType & MBVector<itemType>::get (int x)
{
	if (x >= mySize || x < 0) {
		PANIC("Illegal vector index.");
	}
	
	return myItems[x];
}


template<class itemType>
const itemType & MBVector<itemType>::get (int x) const
{
	if (x >= mySize || x < 0) {
		PANIC("Illegal vector index.");
	}
	return myItems[x];
}

template <class itemType>
itemType & MBVector<itemType>::operator [] (int x)
{
	if (x >= mySize || x < 0) {
		PANIC("Illegal vector index.");
	}
	
	return myItems[x];
}


template<class itemType>
const itemType & MBVector<itemType>::operator [] (int x) const
{
	if (x >= mySize || x < 0) {
		PANIC("Illegal vector index.");
	}
	return myItems[x];
}

template <class itemType>
itemType & MBVector<itemType>::last ()
{
	ASSERT(mySize > 0);
	return myItems[mySize - 1];
}

template <class itemType>
const itemType & MBVector<itemType>::last () const
{
	ASSERT(mySize > 0);
	return myItems[mySize - 1];
}

template <class itemType>
itemType & MBVector<itemType>::first ()
{
	ASSERT(mySize > 0);
	return myItems[0];
}

template <class itemType>
const itemType & MBVector<itemType>::first () const
{
	ASSERT(mySize > 0);
	return myItems[0];
}


template<class itemType>
void MBVector<itemType>::ensureCapacity(int c)
{
	int newCap;
	int minCap;
	
	ASSERT(myCapacity > 0);
	
	if (myCapacity >= c) {
		return;
	}
	
	minCap = myCapacity + c;
	
	newCap = myCapacity;
	while (newCap < minCap) {
		newCap = 2 * newCap + 1;
	}
	ASSERT(newCap > myCapacity);
	
	itemType *t = new itemType[newCap];
	for(int x = 0; x < mySize;x++) {
		t[x] = myItems[x];
	}

	ASSERT(myItems != NULL);
	delete [] myItems;
	
	myCapacity = newCap;
	ASSERT(myCapacity > 0);
	myItems = t;
}

template<class itemType>
void MBVector<itemType>::resize(int newSize)
{
	if (newSize < 0) {
		PANIC("Illegal vector size.");
	}	

	if (newSize <= myCapacity) {
		mySize = newSize;
		return;
	}
	
	ensureCapacity(newSize);
	mySize = newSize;
}

template<class itemType>
void MBVector<itemType>::resize(int newSize,
                                const itemType &fill)
{
	int oldSize = mySize;
	resize(newSize);
	
	while (oldSize < mySize) {
		myItems[oldSize] = fill;
		oldSize++;
	}	
}

template<class itemType>
INLINE void MBVector<itemType>::grow(int howMuch)
{
	ASSERT(howMuch >= 0);
	if (mySize + howMuch > myCapacity) {
		ensureCapacity(mySize + howMuch);
	}
	
	mySize += howMuch;
	ASSERT(mySize >= 0);
	ASSERT(mySize <= myCapacity);
}

template<class itemType>
INLINE void MBVector<itemType>::shrink(int howMuch)
{
	ASSERT(howMuch >= 0);
	ASSERT(mySize - howMuch >= 0);
	mySize -= howMuch;
}

template<class itemType>
const itemType & MBVector<itemType>::pop()
{
	ASSERT(mySize >= 0);
	
	shrink();
	return myItems[mySize];
}

template<class itemType>
bool MBVector<itemType>::isEmpty() const
{
	return size() == 0;
}

template<class itemType>
int MBVector<itemType>::trim()
{
	if (mySize == myCapacity || myCapacity == 1) {
		return 0;
	}
	
	int oup = myCapacity - mySize;
	int newCap = mySize;
	if (newCap == 0) {
		newCap = 1;
	}
	
	itemType *t = new itemType[newCap];
	
	for(int x = 0;x < mySize;x++) {
		t[x] = myItems[x];
	}
	
	myCapacity = newCap;
	delete [] myItems;
	myItems = t;
	
	return oup;
}

template<class itemType>
void MBVector<itemType>::pushAllTo(MBVector<itemType> &v) const
{
	v.ensureCapacity(v.mySize+mySize);
	for(int x=0;x<mySize;x++) {
		v.push(get(x));
	}
}

template<class itemType>
void MBVector<itemType>::pushAllFrom(const MBVector<itemType> &v)
{
	ensureCapacity(v.mySize+mySize);
	for(int x=0;x<v.length();x++) {
		push(v[x]);
	}
}


#endif //MBVector_CPP_201002052320
