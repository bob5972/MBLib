/*
 * MBMap.hpp -- part of MBLib
 *
 * Copyright (c) 2015-2020 Michael Banack <github@banack.net>
 *
 * MIT License
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef MBMAP_H_201001091450
#define MBMAP_H_201001091450

#include <map>

// Basically a wrapper for the std::map class.

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
		void put(const keyType & key, const valueType & val)
        {	
	        myMap[key] = val;
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

