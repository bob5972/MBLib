#ifndef MBQueue_CPP_201001091433
#define MBQueue_CPP_201001091433

#include "MBQueue.h"
#include "mbdebug.h"
#include "mbassert.h"

template<class itemType>
MBQueue<itemType>::MBQueue()
:	mySize(0),
	myTop(NULL),
	myBottom(NULL),
	myLast(NULL)
{
}

template<class itemType>
MBQueue<itemType>::MBQueue(const MBQueue &q)
:	mySize(q.mySize),
	myTop(q.myTop),
	myBottom(q.myBottom),
	myLast(NULL)
{
	if (mySize == 0) {
		return;
	}
	
	myTop = new MBNode(*myTop);
	MBNode* currentNode(myTop);
	MBNode* newNode;
	for (int x=1;x<mySize;x++) {
		newNode = new MBNode(*currentNode->nextNode);
		currentNode->nextNode = newNode;
		currentNode = newNode;
	}
	myBottom = currentNode;
}

template<class itemType>
MBQueue<itemType>::~MBQueue()
{
	makeEmpty();
	if(myLast) {
		delete myLast;
    }

	myLast = NULL;
	//Should be unnecesary...but what the heck
}

template<class itemType>
const MBQueue<itemType>& MBQueue<itemType>::operator = (const MBQueue<itemType> & rhs)
{
	if (this != &rhs) {
		makeEmpty();
		mySize = rhs.mySize;
		myTop = rhs.myTop;
		myBottom = rhs.myBottom;

		if (mySize == 0) {
			return *this;
		}
		
		myTop = new MBNode(*myTop);
		MBNode* currentNode(myTop);
		MBNode* newNode;
		for (int x=1;x<mySize;x++) {
	
			newNode = new MBNode(*currentNode->nextNode);
			currentNode->nextNode = newNode;
			currentNode = newNode;
		}
		myBottom = currentNode;
	}
	return *this;
}

// return top (no dequeue)
template<class itemType>
const itemType& MBQueue<itemType>::front() const
{
    ASSERT(mySize > 0);

	return myTop->value;
}

// insert item (at bottom)
template<class itemType>
void MBQueue<itemType>::enqueue( const itemType &item)
{
	if (mySize > 0) {
		myBottom->nextNode = new MBNode(item,NULL);
		myBottom = myBottom->nextNode;
		mySize++;
	} else {
		myTop = new MBNode(item, NULL);
		myBottom = myTop;
		mySize=1;
	}
}

template<class itemType>
const itemType& MBQueue<itemType>::dequeue( )
{
    ASSERT(mySize > 0);

	if (myLast) {
		delete myLast;
	}
	
	myLast = myTop;
	myTop = myTop->nextNode;
	mySize--;
	return myLast->value;
}

template<class itemType>
void MBQueue<itemType>::dequeue ( itemType &item)
{
	ASSERT(mySize > 0);
	
	item = front();
	dequeue();
}

template<class itemType>
void MBQueue<itemType>::makeEmpty( )
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
	myBottom = NULL;
}	




#endif //MBQueue_CPP_201001091433
