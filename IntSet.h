#ifndef IntSet_H_201002241140
#define IntSet_H_201002241140

#include "IntMap.h"
#include "MBVector.h"

class IntSet
{
	public:
		IntSet();
		IntSet(const IntSet &s);
		
		//returns true iff the set was modified
		bool makeEmpty();
		
		bool contains(int x) const;
		
		bool isEmpty() const;
		int size() const;
		
		//returns true iff the set was modified
		bool insert(int x);
		
		//returns true iff the set was modified
		// Linear time on the size of the set
		bool remove(int x);
		
		//returns true iff the set was modified
		bool insertAll(const IntSet &s);
		
		//pushed onto v
		void getValues(MBVector<int> &v);
	
	private:
		IntMap m;
		MBVector<int> values;
};


#endif //IntSet_H_201002241140
