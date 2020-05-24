/*
 * MBStack.cpp -- part of MBLib
 *
 * Copyright (c) 2015-2020 Michael Banack <bob5972@banack.net>
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
