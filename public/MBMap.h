#ifndef MBMAP_H_201001091450
#define MBMAP_H_201001091450

#include <map>

template <class keyType, class valueType>
class MBMap
{
	public:
		MBMap()
        :myMap()
        {  }
		
		~MBMap()
        {  }

		void clear()
        {
            makeEmpty();
        }

		void makeEmpty()
        {
	        myMap.clear();
        }

		
		bool isEmpty() const
		{
			return size() == 0;
		}

		int size() const
        {
	        return myMap.size();
        }
		
		bool containsKey(const keyType &key) const
        {
	        return myMap.count(key) > 0;
        }
		
		//Returns true if the key was found and removed
		// (false if it was not found)
		bool remove(const keyType&key)
        {
	        return myMap.erase(key) > 0;
        }
										
		const valueType& get(const keyType & key) const
        {
	        //There's probably a better way that'll do this in one query
	        ASSERT(myMap.find(key) != myMap.end());	
	
	        return myMap[key];
        }

		valueType& get(const keyType & key)
        {
	        //There's probably a better way that'll do this in one query
	        ASSERT(myMap.find(key) != myMap.end());	
	
	        return myMap[key];
        }
		
		//Returns true if key already existed (and was replaced)
		//Overrides existing values
		bool put(const keyType & key, const valueType & val)
        {
	        bool oup = containsKey(key);
	
	        myMap[key] = val;
	        return oup;
        }

		//Note that using the index operator
		//  to access a key which is not in the map
		//  will insert a new value initialized with its default
		//  constructor
		valueType & operator [ ] (const keyType & key)
        {
	        return myMap[key];
        }

		const valueType & operator [ ] (const keyType & key) const
        {
	        return myMap[key];
        }
	
	private:
		std::map<keyType, valueType> myMap;
};

#endif //MBMAP_H_201001091450

