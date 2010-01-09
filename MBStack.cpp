#ifndef MBStack_CPP
#define MBStack_CPP

#include "MBStack.h"
#include "mjbdebug.h"
#include "mjbassert.h"

template<class itemType>
MBStack<itemType>::MBStack()
:	mySize(0),
	myTop(NULL),
	myLast(NULL)
{
}

template<class itemType>
MBStack<itemType>::MBStack(const MBStack &q)
:	mySize(q.mySize),
	myTop(q.myTop),
	myLast(NULL)
{
	if (mySize == 0) {
		return;
	}
	
	myTop = new MBNode(*myTop);
	MBNode* currentNode(myTop);
	while (currentNode->nextNode != NULL) {
		currentNode->nextNode = new MBNode(*(myTop->nextNode));
		currentNode = currentNode->nextNode;
	}
}

template<class itemType>
MBStack<itemType>::~MBStack()
{
	makeEmpty();
	if (myLast) {
		delete myLast;
	}
	myLast = NULL;
}

template<class itemType>
const MBStack<itemType>& MBStack<itemType>::operator = (const MBStack<itemType> & rhs)
{
	if (this != &rhs) {
		makeEmpty();
		mySize = rhs.mySize;
		myTop = rhs.myTop;
		
		if (mySize == NULL) {
			return *this;
		}
		myTop = new MBNode(*myTop);
		MBNode* currentNode(myTop);
		MBNode* newNode;
		for (int x=1;x<mySize;x++) {
	
			newNode = new MBNode(*(currentNode->nextNode));
			currentNode->nextNode = newNode;
			currentNode = newNode;
		}
	}
	return *this;
}

// return top (no destack)
template<class itemType>
const itemType& MBStack<itemType>::top() const
{
	if (mySize <=0) {	
		PANIC("Attempted to read from empty stack");
	}
	
	return myTop->value;
}

template<class itemType>
bool MBStack<itemType>::isEmpty() const
{
	return (mySize == 0);
}

template<class itemType>
int MBStack<itemType>::size() const
{
	return mySize;
}

template<class itemType>
void MBStack<itemType>::push( const itemType &item)
{
	if (mySize>0) {
		myTop = new MBNode(item,myTop);
		mySize++;
	} else {
		myTop = new MBNode(item,NULL);
		mySize=1;
	}
}

// remove top element
template<class itemType>
const itemType& MBStack<itemType>::pop()
{
	if (mySize <=0) {	
		PANIC("Attempted to pop an empty stack");
	}
	if (myLast) {
		delete myLast;
	}

	myLast = myTop;
	myTop = myTop->nextNode;
	mySize--;
	return myLast->value;
}

template<class itemType>
void MBStack<itemType>::pop (itemType &item)
{
	if (mySize <=0) {	
		PANIC("Attempted to pop an empty stack");
	}
	item = top();
	pop();
}

template<class itemType>
void MBStack<itemType>::makeEmpty( )
{
	if (!isEmpty()) {
		MBNode* currentNode(myTop);
		MBNode* tempNode;
		while(currentNode->nextNode != NULL) {
			tempNode = currentNode;
			currentNode = tempNode->nextNode;
			delete tempNode;	
		}
		delete currentNode;
	}
	
	mySize = 0;
	myTop = NULL;
}


#endif
