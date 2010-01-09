#ifndef MB_HashSet_H
#define MB_HashSet_H

//NOT TESTED

#include "apvector.h"

template <class itemType>
class MBHashSet
{
	public:
		MBHashSet( int (*hashFunction)(const itemType&));
		MBHashSet(int defSize, int (*hashFunction)(const itemType&));
		MBHashSet(int defSize, double loadFactor, int (*hashFunction)(const itemType&));
		
		void insert(const itemType& item);
		
		void remove(const itemType& item);
		void makeEmpty();
		
		bool isEmpty() const;
		bool contains(const itemType& item) const;
		double load() const;
		double maxLoad() const;
		
		int size() const;
		
		apvector<itemType> items() const;
		
		~MBHashSet();
	
	private:
		class HashEntry
		{
			public:
				HashEntry(const itemType&d)
				:data(d),
				 next(NULL)
				{ }
				
				HashEntry(const itemType&d,HashEntry*n)
				:data(d),
				 next(n)
				{ }
				
				void removeAllChildren()
				{
					if(next==NULL)
						return;
					next->removeAllChildren();
					delete next;
				}
			itemType data;
			HashEntry* next;
		};
		
		//Utility Functions
		static bool isPrime(int x);
		static int smallestPrimeFactor(int x);
		static int nextPrime(int x);
		int hash(const itemType& i) const;
		int find(const itemType& i) const;
		//returns the index of i in myTable or NULL
		//	if i is not in the table
		//Note that the index returned may be a deleted node
		//	(so that insert can reuse array positions if
		//		the same item is removed and reinserted)
		void rehash();
		
		int (*myHashFunction)(const itemType&);
		int mySize;
		double myLoadFactor;
		
		
		apvector< HashEntry* > myTable;
};

#include "MBHashSet.cpp"

#endif
