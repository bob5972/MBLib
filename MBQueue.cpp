#ifndef MBQueue_CPP_201001091433
#define MBQueue_CPP_201001091433

#include "MBQueue.hpp"
#include "mbdebug.h"
#include "mbassert.h"

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
