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
		
		void put(int key, int value);
		
		//returns true iff the map changed
		// (Note that deleting an entry of (1,0) will "change"
		//  the map, even though get(1) will return 0
		//  before an after the delete call
		//  (due to the default value of 0)
		bool remove(int key);
		
		//insert all keys from m into this map
		//  if the key already exists, use the new value
		//  linear time to the capacity (NOT size) of m
		void insertAll(const IntMap &m);
	
	private:
		//positive hash value between 0 and tableSize-1
		int hash(int key) const
		{	
			uint32 hash;
			hash = key;
			hash = hash % mySpace;

			ASSERT(hash >= 0);
			ASSERT(mySpace > 0);
			ASSERT(hash < (uint32) mySpace);
	
			return hash;
		}

        //puts a key at the specified index
        void putHelper(int index, int key, int value);
		
		//returns the index of a valid key, or -1
		int findKey(int key) const;

		//return the index of where this key would be inserted
		//        or -1 if the map is full
		// ie if the key is in the map, return it
		//    if not, return the next free index
		int getInsertionIndex(int key) const;

		//makes the underlying table larger
		void rehash();
		
		static const int DEFAULT_SPACE = 15;
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
