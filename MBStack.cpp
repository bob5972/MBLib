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
