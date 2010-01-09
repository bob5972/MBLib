#ifndef MBMAP_H_201001091450
#define MBMAP_H_201001091450

// NOT TESTED

#include "MBVector.h"

template <class keyType, class valueType>
class MBMap
{
	public:
	//Constructors/destructor
		MBMap();		//Default Constructor - Empty Map
		MBMap(const MBMap &);	//Copy Constructor
		
		~MBMap();		//Destructor
		
	//Assignment
	const MBMap & operator = (const MBMap & map);

	//Accessors
	void clear();	//Empties the Map
	bool isEmpty() const; //Returns true if Empty
	int size() const;		//Returns the size of the map
	bool containsKey(const keyType &key) const;	//Returns true if key is in the map
	bool addKey(const keyType & key, const valueType & val); //Returns true if key is already in map
	                                                        //Overrides existing values
	bool removeKey(const keyType&key);	//Removes a key-value pair from the map
										//Returns false if the key is not in the map, and true otherwise									
	const valueType& getValue(const keyType & key) const;	//Returns the value for the key
	const MBVector<keyType>& keys();	//Returns an apvector containing all the keys in the map
										//Note: No Guaranteed order at this time.
	const MBVector<valueType>& values();//Returns an apvector containing all the values in the map
										//Matches the order in the vector returned by keys
										//ie keys[x] and values[x] are a key-value pair
	
	private:
		int findKey(const keyType &) const;
		MBVector<keyType> myKeys;
		MBVector<valueType> myValues;
			//These should ideally be reimplemented in a struct
			//and a data structure that supports faster sorting
		int mySize;
};

#include "MBMap.cpp"

#endif //MBMAP_H_201001091450
