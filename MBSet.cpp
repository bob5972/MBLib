#ifndef MBSET_CPP_201001091441
#define MBSET_CPP_201001091441

#include "MBSet.h"

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
const MBSet<itemType>& MBSet<itemType>::operator = (const MBSet & set)
{
	myItems = set.myItems;
	mySize = set.mySize;
	isUsed(s.isUsed);
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
	int x(0),seen(0);
	while (x<myItems.length() && seen<mySize) {
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
bool MBSet<itemType>::insert(const itemType&t)
{
	if (!contains(t)) {
		if (mySize == myItems.length()) {
			myItems.resize(mySize*2);
			isUsed.resize(mySize*2);
		}
		int x(0);
		while (x<myItems.length()) {
			if(!isUsed[x]) 	{
				isUsed[x] = true;
				myItems[x] = t;
				mySize++;
				return false;
			}
			x++;
		}
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
	while (x<myItems.length() && seen<mySize) {
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
	apvector<itemType> oup(mySize);
	int x(0),seen(0);
	while (seen<mySize) {
		if (isUsed[x]) {
			oup[seen++] = myItems[x];
		}
	}
	return oup;
}

#endif //MBSET_CPP_201001091441
