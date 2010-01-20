#ifndef IntMap_H_201001201109
#define IntMap_H_201001201109

#include "MBVector.h"
#include "BitArray.h"

class IntMap
{
	public:
		IntMap();		
		explicit IntMap(int space);		
		IntMap(int space, double load);
		IntMap(const IntMap& m);
		
		void makeEmpty();
		
		bool containsKey(int key) const;
		bool isEmpty() const;
		int size() const;
		
		//defaults to a value of 0 for missing keys
		int get(int key) const;
		
		//returns the new value
		int increment(int key);
		//returns the new value
		int decrement(int key);
		
		void put(int key, int value);
		
		//returns true iff the key was present
		bool remove(int key);
	
	private:
		//positive hash value between 0 and tableSize-1
		int hash(int key) const;
		
		//returns the index of a valid key, or -1
		int findKey(int key) const;
		
		//returns the index where key would go, or -1 if full
		// ie, if key is in the map, this will return the index
		//     if not, it will return an empty index
		int getIndexOfKey(int key) const;
		
		//makes the underlying table larger
		void rehash();
		
		static const int DEFAULT_SPACE = 7;
		static const double DEFAULT_LOAD = 0.75;
		static const int SEARCH_INCR = 2;
		
		MBVector<int> myKeys;
		MBVector<int> myValues;
		
		//is the entry "active"
		BitArray myActiveFlags;
		//is the entry "full" (for hashing purposes)
		BitArray myFullFlags;
		
		int mySize;
		int mySpace;
		int myFreeSpace;
		double myLoad;
	
	
};







#endif //IntMap_H_201001201109
