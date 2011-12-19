#ifndef IntMap_H_201001201109
#define IntMap_H_201001201109

#include "MBVector.h"
#include "BitVector.h"

class IntMap
{
	public:
		IntMap();		
		explicit IntMap(int space);		
		IntMap(int space, double load);
		IntMap(const IntMap& m);
		
		void makeEmpty();
		
		bool containsKey(int key) const
		{
			return findKey(key) != -1;
		}

		bool isEmpty() const {
			return mySize == 0;
		}
		
		int size() const {
			return mySize;
		}
		
		//defaults to a value of 0 for missing keys
		int get(int key) const
		{
			int i = findKey(key);
			
			if( i == -1) {
				return 0;
			}
			
			return myValues[i];
		}

		
		//returns the new value
		int increment(int key) {
			return increment(key, 1);
		}
		
		//returns the new value
		int decrement(int key) {
			return decrement(key, 1);
		}
		
		//returns the new value
		int increment(int key, int amount);
		//returns the new value
		int decrement(int key, int amount);
		
		//return true iff the map changed
		bool put(int key, int value);
		
		//returns true iff the map changed
		// (Note that deleting an entry of (1,0) will "change"
		//  the map, even though get(1) will return 0
		//  before an after the delete call
		//  (due to the default value of 0)
		bool remove(int key);
		
		//insert all keys from m into this map
		//  if the key already exists, use the new value
		//  linear time to the capacity (NOT size) of m
		bool insertAll(const IntMap &m);
	
	private:
		//positive hash value between 0 and tableSize-1
		int hash(int key) const
		{	
			int hash;
			hash = key;
			hash = hash % mySpace;
			if(hash < 0) {
				hash = -hash;
			}

			ASSERT(hash >= 0);
			ASSERT(hash < mySpace);
	
			return hash;
		}
		
		//returns the index of a valid key, or -1
		int findKey(int key) const;
		
		//returns the index where key would go, or -1 if full
		// ie, if key is in the map, this will return the index
		//     if not, it will return an empty index
		int getIndexOfKey(int key) const;
		
		//returns the first free index in this key's
		// hash sequence, or -1 if full
		// ie, calling this function for an insertion
		//  presupposes the key is not in the table
		int getFreeIndex(int key) const;
		
		//makes the underlying table larger
		void rehash();
		
		static const int DEFAULT_SPACE = 7;
		static const double DEFAULT_LOAD = 0.75;
		static const int SEARCH_INCR = 2;
		
		MBVector<int> myKeys;
		MBVector<int> myValues;
		
		//is the entry "active"
		BitVector myActiveFlags;
		//is the entry "full" (for hashing purposes)
		BitVector myFullFlags;
		
		int mySize;
		int mySpace;
		int myFreeSpace;
		double myLoad;	
};



#endif //IntMap_H_201001201109
