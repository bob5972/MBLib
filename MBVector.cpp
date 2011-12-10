#ifndef MBVector_CPP_201002052320
#define MBVector_CPP_201002052320

#include <stdlib.h>
#include "MBVector.h"
#include "mbdebug.h"
#include "mbassert.h"

template<class itemType>
MBVector<itemType>::MBVector()
:mySize(0),
 myCapacity(0),
 myItems(NULL)
{}

template<class itemType>
MBVector<itemType>::MBVector(int size)
:mySize(size),
 myCapacity(size),
 myItems(new itemType[size])
{}

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
}

template <class itemType>
void MBVector<itemType>::consume(MBVector<itemType> &v)
{
	makeEmpty();
	
	myItems = v.myItems;
	mySize = v.mySize;
	myCapacity = v.mySize;
	
	v.myItems = NULL;
	v.mySize = 0;
	v.myCapacity = 0;
}

template <class itemType>
const MBVector<itemType> &
	MBVector<itemType>::operator = (const MBVector<itemType> & rhs)
{
	if (this != &rhs) {
		resize(rhs.mySize);
		for (int x=0;x<mySize;x++) {
			myItems[x] = rhs.myItems[x];
		}
	}
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

template<class itemType>
int MBVector<itemType>::length() const
{
	return mySize;
}

template<class itemType>
int MBVector<itemType>::size() const
{
	return mySize;
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
void MBVector<itemType>::resize(int newSize)
{
	if (newSize < 0) {
		PANIC("Illegal vector size.");
	}	

	if (newSize < myCapacity) {
		mySize = newSize;
		return;
	}

	if (newSize == 0) {
		if (myCapacity > 0) {
			delete[] myItems;
		}
		myItems = NULL;
		mySize = myCapacity = newSize;//=0
		return;
	}
	
	itemType *t = new itemType[newSize];

	for(int x=0;x<mySize;x++) {
		t[x] = myItems[x];
	}

	if (myCapacity > 0) {
		delete [] myItems;
	}
	
	myCapacity = mySize = newSize;
	myItems = t;
	return;
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
void MBVector<itemType>::grow()
{
	if (mySize < myCapacity) {
		mySize++;
		return;
	}
	
	int oldSize = mySize;
	resize(myCapacity>0?myCapacity*2:2);
	mySize = oldSize+1;
}

template<class itemType>
void MBVector<itemType>::grow(int howMuch)
{
	while (howMuch > 0) {
		grow();
		howMuch--;
	}
	
	while (howMuch < 0) {
		shrink();
		howMuch++;
	}
}

template<class itemType>
void MBVector<itemType>::shrink()
{
	mySize--;
	if (mySize < 0) {
		PANIC("Vector cannot shrink smaller than size 0.");
	}
}

template<class itemType>
void MBVector<itemType>::shrink(int howMuch)
{
	grow(-howMuch);
}

template<class itemType>
int MBVector<itemType>::push(const itemType & item)
{
	grow();
	myItems[mySize-1] = item;
	return mySize-1;
}

template<class itemType>
const itemType & MBVector<itemType>::pop()
{
	if (mySize <= 0) {
		PANIC("Cannot pop an empty vector.");
	}	
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
	if (mySize == myCapacity) {
		return 0;
	}
	
	int oup = myCapacity - mySize;
	itemType *t = new itemType[mySize];
	
	for(int x=0;x<mySize;x++) {
		t[x] = myItems[x];
	}
	
	myCapacity = mySize;
	delete [] myItems;
	myItems = t;
	return oup;
}

template<class itemType>
void MBVector<itemType>::pushAllTo(MBVector<itemType> &v) const
{
	v.ensureCapacity(v.length()+length());
	for(int x=0;x<length();x++) {
		v.push(get(x));
	}
}

template<class itemType>
void MBVector<itemType>::pushAllFrom(const MBVector<itemType> &v)
{
	ensureCapacity(v.length()+length());
	for(int x=0;x<v.length();x++) {
		push(v[x]);
	}
}

template<class itemType>
void MBVector<itemType>::ensureCapacity(int c)
{
	if(myCapacity < c) {
		int oldSize = mySize;
		resize(c);
		mySize = oldSize;
	}	
}

#endif //MBVector_CPP_201002052320
