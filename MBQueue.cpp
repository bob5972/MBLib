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
