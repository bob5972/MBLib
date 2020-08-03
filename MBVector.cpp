/*
 * MBVector.cpp -- part of MBLib
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

#ifndef MBVector_CPP_201002052320
#define MBVector_CPP_201002052320

#include <stdlib.h>
#include "MBVector.hpp"
#include "mbdebug.h"
#include "mbassert.h"
#include "mbutil.h"

template<class itemType>
MBVector<itemType>::MBVector(const MBVector<itemType>& vec)
:mySize(vec.mySize),
 myCapacity(mySize),
 myPinCount(0),
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
 myPinCount(0),
 myItems(new itemType[size])
{
	for (int x=0;x<size;x++)
	{
		myItems[x] = fillValue;
	}

	ASSERT(myCapacity > 0);
}

template <class itemType>
void MBVector<itemType>::consume(MBVector<itemType> &v)
{
    ASSERT(myPinCount == 0);
    ASSERT(v.myPinCount == 0);

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


template<class itemType>
void MBVector<itemType>::ensureCapacity(int c)
{
	int newCap;
	int minCap;

	ASSERT(myCapacity > 0);

	if (myCapacity >= c) {
		return;
	}

	ASSERT(myPinCount == 0);

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
int MBVector<itemType>::trim()
{
	if (mySize == myCapacity || myCapacity == 1) {
		return 0;
	}

	ASSERT(myPinCount == 0);

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
