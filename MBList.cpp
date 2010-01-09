#ifndef MBLIST_CPP
#define MBLIST_CPP

#include "MBList.h"

template<class itemType>
MBList<itemType>::MBList()
:	myHead(NULL),
	myTail(NULL),
	mySize(0)
{
}

template<class itemType>
MBList<itemType>::MBList(int size, const itemType & fillValue)
:	myHead(NULL),
	myTail(NULL),
	mySize(0)
{
	if (size < 0)
	{
		cerr << "Invalid Size: "<<size<<" Min Size = 0"<<endl;
		abort();
	}
	while (mySize != size)
	{
		prepend(fillValue);
	}
}

template<class itemType>
MBList<itemType>::MBList(int size)
:	myHead(NULL),
	myTail(NULL),
	mySize(0)
{
	resize(size);
}

template<class itemType>
MBList<itemType>::MBList(const MBList &orig)
:	mySize(orig.mySize),
{
	myHead = new MBNode<itemType>(*orig.myHead);
	MBNode<itemType>* CNode;
	CNode = myHead;
	while (CNode->nextNode != NULL)
	{
		CNode->nextNode = new MBNode<itemType>(*CNode->nextNode);
	}
	myTail = CNode;
}

template<class itemType>
MBList<itemType>::MBList(const apvector<itemType> &orig)
:	mySize(0),
	myHead(NULL),
	myTail(NULL)
{
	for (int i =0;i<orig.length();i++)
	{
		append(orig[i]);
	}
}

template<class itemType>
int MBList<itemType>::length() const
{
	return mySize;
}


template<class itemType>
void MBList<itemType>::makeEmpty()
{
	if (mySize != 0)
	{
		MBNode<itemType>* start(myHead);
		MBNode<itemType>* next(start->nextNode);
		while (next != NULL)
		{
			delete start;
			start = next;
			next = next->nextNode;
		}
		delete start;
	}
	mySize = 0;
	myHead = NULL;
	myTail = NULL;
}

template<class itemType>
MBList<itemType>::~MBList()
{
	makeEmpty();
}

template<class itemType>
itemType& MBList<itemType>::operator [ ] (int index)
//Range Checked Indexing
{
	if (!indexInRange(index))
	{
		cerr << "MBList Invalid Index: "<<index<<" Max Index: "<<mySize-1<<endl;
		abort();
	}
	MBNode<itemType>* current(myHead);
	for (int x=0;x<index;x++)
	{
		current = current->nextNode;
	}
	return current->value;
}


template<class itemType>
const itemType& MBList<itemType>::operator [ ] (int index) const
//Range Checked Indexing
{
	if (!indexInRange(index))
	{
		cerr << "MBList Invalid Index: "<<index<<" Max Index: "<<mySize-1<<endl;
		abort();
	}
	MBNode<itemType>* current(myHead);
	for (int x=0;x<index;x++)
	{
		current = current->nextNode;
	}
	return current->value;
}

template<class itemType>
void MBList<itemType>::prepend(const itemType &val)
//Adds val to beginning of list
{
	if (mySize == 0)
	{
		mySize++;
		myHead = new MBNode<itemType>(val);
		myTail = myHead;
		return;
	}
	myHead = new MBNode<itemType>(val,myHead);
	mySize ++;
}

template<class itemType>
void MBList<itemType>::append(const itemType &val)
//Adds val to end of list
{
	if (mySize == 0)
	{
		mySize++;
		myHead = new MBNode<itemType>(val);
		myTail = myHead;
		return;
	}
	myTail->nextNode = new MBNode<itemType>(val);
	myTail = myTail->nextNode;
	mySize++;
}

template<class itemType>
void MBList<itemType>::remove(int index)
{
	if (!indexInRange(index))
	{
		cerr << "MBList Invalid Index: "<<index<<" Max Index: "<<mySize-1<<endl;
		abort();
	}
	if (index == 0)
	{
		mySize--;
		MBNode<itemType>* next(myHead->nextNode);
		delete myHead;
		myHead = next;
		return;
	}
	if (index == mySize-1)
	{
		MBNode<itemType>* before(findNode(index-1));
		before->nextNode = NULL;
		delete myTail;
		myTail = before;
		return;
	}
	MBNode<itemType>* before(findNode(index-1));
	MBNode<itemType>* badOne(before->nextNode);
	before->nextNode = badOne->nextNode;
	mySize--;
	delete badOne;
}

template<class itemType>
void MBList<itemType>::insertBefore(const itemType& val,int index)
//Inserts val before index
{
	if (!indexInRange(index))
	{
		cerr << "MBList Invalid Index: "<<index<<" Max Index: "<<mySize-1<<endl;
		abort();
	}
	if (index==0)
	{
		prepend(val);
		return;
	}
	MBNode<itemType>* first(findNode(index-1));
	MBNode<itemType>* last(first->nextNode);
	first->nextNode = new MBNode<itemType>(val,last);
	mySize++;
}

template<class itemType>
void MBList<itemType>::insertAfter(const itemType& val,int index)
//Inserts val after index
{
	if (!indexInRange(index))
	{
		cerr << "MBList Invalid Index: "<<index<<" Max Index: "<<mySize-1<<endl;
		abort();
	}
	if (index==mySize-1)
	{
		append(val);
		return;
	}
	MBNode<itemType>* first(findNode(index));
	MBNode<itemType>* last(first->nextNode);
	first->nextNode = new MBNode<itemType>(val,last);
	mySize++;
}

template<class itemType>
MBNode<itemType>* MBList<itemType>::findNode(int index) const
{
	if (!indexInRange(index))
	{
		cerr << "MBList Invalid Index: "<<index<<" Max Index: "<<mySize-1<<endl;
		abort();
	}
	MBNode<itemType>* current(myHead);
	for (int x=0;x<index;x++)
	{
		current = current->nextNode;
	}
	return current;
}

template<class itemType>
int MBList<itemType>::findIndex(const MBNode<itemType> *theOne) const
{
	int index=0;
	MBNode<itemType>* current(myHead);
	while (current!=theOne && current != NULL)
	{
		current = current->nextNode;
		index++;
	}
	if (current = theOne)
		return index;
	return -1;
}
		
template<class itemType>
bool MBList<itemType>::indexInRange(int index) const
{
	return (index >= 0 && index < mySize);
}

template<class itemType>
void MBList<itemType>::resize(int newSize)
{
	if (newSize < 0)
	{
		cerr << "Invalid Resize of MBList: "<< newSize<< " Min Size = 0"<<endl;
		abort();
	}
	if (newSize == 0)
	{
		makeEmpty();
		return;
	}
	if (newSize < mySize)
	{
		while (newSize != mySize)
		{
			remove(mySize-1);
		}
		return;
	}
	if (newSize > mySize)
	{
		if (mySize == 0)
		{
			myHead = new MBNode<itemType>();
			myTail = myHead;
			mySize++;
		}
			
		while (newSize != mySize)
		{
			myTail->nextNode = new MBNode<itemType>();
			myTail = myTail->nextNode;
			mySize++;
		}
		return;
	}
}

template <class itemType>			
const MBList<itemType>& MBList<itemType>::operator = ( const MBList<itemType> & rhs)
{
	mySize = rhs.mySize;
	myHead = new MBNode<itemType>(*rhs.myHead);
	MBNode<itemType>* CNode;
	CNode = myHead;
	while (CNode->nextNode != NULL)
	{
		CNode->nextNode = new MBNode<itemType>(*CNode->nextNode);
	}
	myTail = CNode;
	return *this;
}

template<class itemType>
const MBList<itemType>& MBList<itemType>::operator = ( const apvector<itemType> & rhs)
{
	makeEmpty();
	for (int x=0;x<rhs.length();x++)
	{
		append(rhs[x]);
	}
	return *this;
}
	
	
#endif