/*
 * MBQueue.cpp -- part of MBLib
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
