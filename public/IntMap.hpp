/*
 * This file is part of MBLib.
 * Copyright (c) 2015 Michael Banack <bob5972@banack.net>
 *
 * MBLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MBLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MBLib.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef IntMap_HPP_201001201109
#define IntMap_HPP_201001201109

#ifndef __cplusplus
#error Including C++ Header in a C file.
#endif

#define IntMap IntMapData
extern "C" {
#include "IntMap.h"
}
#undef IntMap

class IntMap {
	public:
		IntMap() {
		    IntMap_Create(&data);
		}

		IntMap(const IntMap &m) {
		    IntMap_Create(&data);
		    IntMap_InsertAll(&data, &m.data);
		}

		void makeEmpty() {
		    IntMap_MakeEmpty(&data);
		}

		bool containsKey(int key) const
		{
                    return IntMap_ContainsKey(&data, key);
		}

		bool isEmpty() const {
                    return IntMap_IsEmpty(&data);
		}

		int size() const {
                    return IntMap_Size(&data);
		}

		//defaults to a value of 0 for missing keys
		int get(int key) const
		{
                    return IntMap_Get(&data, key);
		}


		//returns the new value
		int increment(int key) {
                    return IntMap_Increment(&data, key);
		}

		//returns the new value
		int decrement(int key) {
                    return IntMap_Decrement(&data, key);
		}

		//returns the new value
		int increment(int key, int amount) {
		    return IntMap_IncrementBy(&data, key, amount);
		}
		//returns the new value
		int decrement(int key, int amount) {
		    return IntMap_DecrementBy(&data, key, amount);
		}

		void put(int key, int value) {
		    IntMap_Put(&data, key, value);
		}

		//returns true iff the map changed
		// (Note that deleting an entry of (1,0) will "change"
		//  the map, even though get(1) will return 0
		//  before an after the delete call
		//  (due to the default value of 0)
		bool remove(int key) {
		    return IntMap_Remove(&data, key);
		}

		//insert all keys from m into this map
		//  if the key already exists, use the new value
		//  linear time to the capacity (NOT size) of m
		void insertAll(const IntMap &m) {
		    return IntMap_InsertAll(&data, &m.data);
		}

	private:
		IntMapData data;
};


#endif //IntMap_HPP_201001201109
