/*
 * This file is part of MBLib.
 * Copyright (c) 2015 Michael Banack <bob5972@banack.net>
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


#ifndef MBStack_CPP
#define MBStack_CPP

#include "MBStack.hpp"
#include "mbdebug.h"
#include "mbassert.h"

template<class itemType>
MBStack<itemType>::MBStack()
:myVector(0)
{
}

template<class itemType>
MBStack<itemType>::MBStack(const MBStack &q)
:myVector(q.myVector)
{
}

template<class itemType>
MBStack<itemType>::~MBStack()
{
}

template<class itemType>
const MBStack<itemType>&
MBStack<itemType>::operator = (const MBStack<itemType> & rhs)
{
	if (this != &rhs) {
		myVector = rhs.myVector;
	}
	return *this;
}

// return top (no destack)
template<class itemType>
const itemType& MBStack<itemType>::top() const
{
	return myVector.last();
}

template<class itemType>
bool MBStack<itemType>::isEmpty() const
{
	return myVector.isEmpty();
}

template<class itemType>
int MBStack<itemType>::size() const
{
	return myVector.length();
}

template<class itemType>
void MBStack<itemType>::push( const itemType &item)
{
	myVector.push(item);
}

// remove top element
template<class itemType>
const itemType& MBStack<itemType>::pop()
{
	return myVector.pop();
}

template<class itemType>
void MBStack<itemType>::pop (itemType &item)
{
	item = myVector.pop();
}

template<class itemType>
void MBStack<itemType>::makeEmpty( )
{
	myVector.makeEmpty();
}

template<class itemType>
void MBStack<itemType>::trim( )
{
	myVector.trim();
}


#endif
