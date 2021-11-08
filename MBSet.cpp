/*
 * MBSet.cpp -- part of MBLib
 *
 * Copyright (c) 2015-2021 Michael Banack <github@banack.net>
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

#ifndef MBSET_CPP_201001091441
#define MBSET_CPP_201001091441

#include "MBSet.hpp"
#include "MBDebug.h"

template<class itemType>
MBSet<itemType>::MBSet()
:	myItems(1),
	isUsed(1,false),
	mySize(0)
{
}

template<class itemType>
MBSet<itemType>::MBSet(const MBSet &s)
:	myItems(s.myItems),
	isUsed(s.isUsed),
	mySize(s.mySize)
{
}

template<class itemType>
MBSet<itemType>::~MBSet()
{
}

template<class itemType>
const MBSet<itemType>& MBSet<itemType>::operator = (const MBSet &rhs)
{
	myItems = rhs.myItems;
	mySize = rhs.mySize;
	isUsed = rhs.isUsed;
    return *this;
}

template<class itemType>
void MBSet<itemType>::makeEmpty()
{
	myItems.resize(0);
	isUsed.resize(0);
	mySize = 0;
}

template<class itemType>
bool MBSet<itemType>::isEmpty() const
{
	return (size() == 0);
}

template<class itemType>
int MBSet<itemType>::size() const
{
	return mySize;
}

template<class itemType>
bool MBSet<itemType>::contains(const itemType&t) const
{
	int x = 0;
	int seen = 0;

	ASSERT(myItems.size() == isUsed.size());

	while (x < myItems.size() && seen < mySize) {
		if (isUsed[x]) {
			if(myItems[x] == t) {
				return true;
			}
			seen++;
		}
		x++;
	}
	return false;
}

template<class itemType>
bool MBSet<itemType>::insert(const itemType & t)
{
	if (!contains(t)) {
		int length = myItems.size();
		ASSERT(length == isUsed.size());
		ASSERT(length > 0);

		if (mySize + 1 >= length) {
			myItems.resize(length * 2);
			isUsed.setFillValue(FALSE);
			isUsed.resize(length * 2);
		}

		int x = 0;
		length = myItems.size();
		ASSERT(mySize + 1 < length);
		while (x < length) {
			if(!isUsed[x]) 	{
				isUsed.set(x);
				myItems[x] = t;
				mySize++;
				return false;
			}
			x++;
		}
		NOT_REACHED();
	}
	return true;
}

template<class itemType>
bool MBSet<itemType>::add(const itemType&t)
{
	return insert(t);
}

template<class itemType>
bool MBSet<itemType>::remove(const itemType &t)
{
	int x(0),seen(0);
	while (x < myItems.length() && seen < mySize) {
		if (isUsed[x]) 	{
			if(myItems[x] == t) 		{
				isUsed[x] = false;
				mySize--;
				return true;
			}
			seen++;
		}
		x++;
	}
	return false;
}

template<class itemType>
const MBVector<itemType>& MBSet<itemType>:: items() const
{
	MBVector<itemType> oup(mySize);
	int x(0),seen(0);
	while (seen<mySize) {
		if (isUsed[x]) {
			oup[seen++] = myItems[x];
		}
	}
	return oup;
}

#endif //MBSET_CPP_201001091441
