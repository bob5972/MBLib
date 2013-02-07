#ifndef IntSet_H_201002241140
#define IntSet_H_201002241140

#include "IntMap.hpp"
#include "MBVector.hpp"

class IntSet
{
	public:
		IntSet() {}
		IntSet(const IntSet &s) {
			insertAll(s);
		}

		//returns true iff the set was modified
		bool makeEmpty();

		bool contains(int x) const {
			return m.get(x) == 1;
		}

		bool isEmpty() const {
			return size() == 0;
		}

		int size() const {
			return values.size();
		}

		//returns true iff the set was modified
		bool insert(int x);

		bool add(int x) {
			return insert(x);
		}

		//returns true iff the set was modified
		// Linear time on the size of the set
		bool remove(int x);

		//returns true iff the set was modified
		bool insertAll(const IntSet &s);

		//pushed onto v
		void getValues(MBVector<int> &v) const;

	private:
		IntMap m;
		MBVector<int> values;

		bool checkInvariants() const;
};


#endif //IntSet_H_201002241140
