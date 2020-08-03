/*
 * IntMap.hpp -- part of MBLib
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


#ifndef IntMap_HPP_201001201109
#define IntMap_HPP_201001201109

#ifndef __cplusplus
#error Including C++ Header in a C file.
#endif

extern "C" {
#include "IntMap.h"
}

class IntMap {
	public:
		IntMap() {
		    CIntMap_Create(&myData);
		}

		IntMap(const IntMap &m) {
		    CIntMap_Create(&myData);
		    CIntMap_InsertAll(&myData, &m.myData);
		}

		void setEmptyValue(int emptyValue) {
            CIntMap_SetEmptyValue(&myData, emptyValue);
        }

		void makeEmpty() {
		    CIntMap_MakeEmpty(&myData);
		}

		bool containsKey(int key) const
		{
            return CIntMap_ContainsKey(&myData, key);
		}

		bool isEmpty() const {
            return CIntMap_IsEmpty(&myData);
		}

		int size() const {
            return CIntMap_Size(&myData);
		}

		//defaults to a value of 0 for missing keys
		int get(int key) const
		{
            return CIntMap_Get(&myData, key);
		}


		//returns the new value
		int increment(int key) {
            return CIntMap_Increment(&myData, key);
		}

		//returns the new value
		int decrement(int key) {
            return CIntMap_Decrement(&myData, key);
		}

		//returns the new value
		int increment(int key, int amount) {
		    return CIntMap_IncrementBy(&myData, key, amount);
		}
		//returns the new value
		int decrement(int key, int amount) {
		    return CIntMap_DecrementBy(&myData, key, amount);
		}

		void put(int key, int value) {
		    CIntMap_Put(&myData, key, value);
		}

		//returns true iff the map changed
		// (Note that deleting an entry of (1,0) will "change"
		//  the map, even though get(1) will return 0
		//  before an after the delete call
		//  (due to the default value of 0)
		bool remove(int key) {
		    return CIntMap_Remove(&myData, key);
		}

		//insert all keys from m into this map
		//  if the key already exists, use the new value
		//  linear time to the capacity (NOT size) of m
		void insertAll(const IntMap &m) {
		    return CIntMap_InsertAll(&myData, &m.myData);
		}

	private:
		CIntMap myData;
};


#endif //IntMap_HPP_201001201109
