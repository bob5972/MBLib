#ifndef MBMAP_H_201001091450
#define MBMAP_H_201001091450

#include <map>

template <class keyType, class valueType>
class MBMap
{
	public:
		MBMap();
		
		~MBMap();

		void clear();
		void makeEmpty();
		
		bool isEmpty() const;
		int size() const;
		
		bool containsKey(const keyType &key) const;
		
		//Returns true if the key was found and removed
		// (false if it was not found)
		bool remove(const keyType&key);
										
		const valueType& get(const keyType & key) const;
		valueType& get(const keyType & key);
		
		//Returns true if key already existed (and was replaced)
		//Overrides existing values
		bool put(const keyType & key, const valueType & val);

		//Note that using the index operator
		//  to access a key which is not in the map
		//  will insert a new value initialized with its default
		//  constructor
		valueType & operator [ ] (const keyType & key);
		const valueType & operator [ ] (const keyType & key) const;
	
	private:
		std::map<keyType, valueType> myMap;
};

#include "MBMap.cpp"

#endif //MBMAP_H_201001091450

