#ifndef MBSET_H_201001091440
#define MBSET_H_201001091440

#include "MBVector.h"

template <class itemType>
class MBSet
{
	public:
		MBSet();
		MBSet(const MBSet &);
		
		~MBSet();
		
		const MBSet & operator = (const MBSet & set);
		
		//Accessors
		void makeEmpty();
		bool isEmpty() const;
		int size() const;
		bool contains(const itemType &) const;
		bool add(const itemType &);
		bool insert(const itemType &);
		bool remove(const itemType &);
		const MBVector<itemType>& items() const;
		
	private:
		apvector<itemType> myItems;
		apvector<bool> isUsed;
		
		int mySize;
};




#include "MBSet.cpp"
#endif //MBSET_H_201001091440
