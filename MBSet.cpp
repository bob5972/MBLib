/*
 * This file is part of MBLib.
 * Copyright (c) 2015-2019 Michael Banack <bob5972@banack.net>
 *
 * MBLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MBLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MBLib.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef MBSET_CPP_201001091441
#define MBSET_CPP_201001091441

#include "MBSet.hpp"
#include "mbdebug.h"

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
