#ifndef MBLIST_H
#define MBLIST_H

//NOT TESTED

#include "MBNode.h"
#include <stdlib.h>

template<class item>
class apvector;

template<class itemType>
class MBList
{
	public:
		//Constructors
		MBList();
			//Default Constructor
			//Postcondition: Empty List
		MBList(int size, const itemType & fillValue);
			//List of length size filled with fillValue
		MBList(int size);
			//Uninitialized list of length size
		MBList(const MBList &orig);
			//Copy Constructor
			//Postcondition: copy of orig returned
		MBList(const apvector<itemType> &);
			//Postcondition: vector converted to list
		~MBList();
			//Postcondition: memory freed, list destructed
		
		// assignment
		const MBList & operator = ( const MBList & rhs);
		const MBList & operator = ( const apvector<itemType> & rhs);
		
		//Accessors
		int length() const;
			//Returns # of elements
			
		//Indexing
		itemType & 	operator [ ] (int index);				//Range Checked Indexing
		const itemType & operator [ ] (int index) const;	//Range Checked Indexing
		

		//Modifiers
		void resize(int newSize);
			//Enlages or shrinks list
		void makeEmpty();
			//Removes all elements
		void prepend(const itemType &val);
			//Adds val to beginning of list
		void append(const itemType &val);
			//Adds val to end of list
		void remove(int index);
			//Removes node at index from list
		void insertBefore(const itemType &val,int index);
			//Inserts val before index
		void insertAfter(const itemType &val, int index);
			//Inserts val after index
			
	private:
		MBNode<itemType>* myHead;
		MBNode<itemType>* myTail;

		int mySize;
		
		MBNode<itemType>* findNode(int index) const;
		int findIndex(const MBNode<itemType> *) const;
		bool indexInRange(int index) const;
};


#endif
#include "MBlist.cpp"
