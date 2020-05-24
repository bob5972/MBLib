/*
 * MBVector.hpp -- part of MBLib
 *
 * Copyright (c) 2015-2020 Michael Banack <bob5972@banack.net>
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

#ifndef MBVECTOR_HPP_201001091353
#define MBVECTOR_HPP_201001091353

#include "mbassert.h"

/*
 * The C++ version of this doesn't actually use
 * the C version, because I couldn't figure out
 * how to get [] operators to work.
 *
 * (The compiler wouldn't let me pass references
 * out to regular pointers).
 */
extern "C" {
#define MBVector MBVectorData
#include "MBVector.h"
#undef MBVector
}

template<class itemType>
class MBVector
{
    public:

        //Constructors
        MBVector()
        : mySize(0), myCapacity(1), myItems(new itemType[1])
        {
            ASSERT(myCapacity > 0);
        }

        MBVector(const MBVector& vec);

        //Default Vector of length size
        explicit MBVector(int size)
        {
            mySize = size;
            ASSERT(mySize >= 0);

            if (mySize >= 1) {
                myCapacity = mySize;
            } else {
                myCapacity = 1;
            }

            myItems = new itemType[myCapacity];

            ASSERT(myCapacity > 0);
        }

        //Vector of length size initialized to fillValue
        MBVector(int size, const itemType & fillValue);

        //Destructor
        ~MBVector()
        {
            delete[] myItems;
        }

        //Assignment
        const MBVector & operator =(const MBVector & vec);

        //Accessors
        //return the leftmost index of item
        int find(const itemType & item) const;

        int size() const
        {
            return mySize;
        }

        bool isEmpty() const
        {
            return size() == 0;
        }

        void makeEmpty()
        {
            resize(0);
            ASSERT(myCapacity > 0);
        }

        //Indexing
        itemType & get(int x)
        {
            ASSERT(x < mySize);
            ASSERT(x >= 0);

            return myItems[x];
        }

        const itemType & get(int x) const
        {
            ASSERT(x < mySize);
            ASSERT(x >= 0);

            return myItems[x];
        }

        itemType & operator [](int x)
        {
            return get(x);
        }

        const itemType & operator [](int x) const
        {
            return get(x);
        }

        const itemType &last() const
        {
            ASSERT(mySize > 0);
            return myItems[mySize - 1];
        }

        itemType &last()
        {
            ASSERT(mySize > 0);
            return myItems[mySize - 1];
        }

        const itemType &first() const
        {
            ASSERT(mySize > 0);
            return myItems[0];
        }

        itemType &first()
        {
            ASSERT(mySize > 0);
            return myItems[0];
        }

        //Modifiers
        //Resizes vector to length newSize
        void resize(int newSize);

        //Resize with new items initialized to fill value
        void resize(int newSize, const itemType &fill);

        //Increases the vector size by 1
        void grow()
        {
            grow(1);
        }

        //Increases the vector size by howMuch
        void grow(int howMuch)
        {
            ASSERT(howMuch >= 0);
            if (mySize + howMuch > myCapacity) {
                ensureCapacity(mySize + howMuch);
            }

            mySize += howMuch;
            ASSERT(mySize >= 0);
            ASSERT(mySize <= myCapacity);
        }

        //Decreases vector size by 1
        void shrink()
        {
            shrink(1);
        }

        //Decreases vector size by howMuch
        //Calling shrink past size 0 results in an error
        void shrink(int howMuch)
        {
            ASSERT(howMuch >= 0);
            ASSERT(mySize - howMuch >= 0);
            mySize -= howMuch;
        }

        //Adds item to the end of the vector
        //returns new largest valid index (old size)
        int push(const itemType & item)
        {
            grow();
            myItems[mySize - 1] = item;
            return mySize - 1;
        }

        //returns the item on the end of the vector and decreases the size by 1
        //ie (the last item pushed)
        const itemType & pop()
        {
            ASSERT(mySize >= 0);

            shrink();
            return myItems[mySize];
        }

        //This is a weird function.
        //It empties this vector, copies over everything from
        //  v, and then empties v.
        void consume(MBVector<itemType> &v);

        //pushes all the elements of v into this vector
        void pushAllFrom(const MBVector<itemType> & v);

        //pushes all the elements of this vector into v
        void pushAllTo(MBVector<itemType> & v) const;

        //frees excess array locations
        //(ie more available memory)
        //post: myCapacity == mySize
        //returns the number of freed indexes
        //(ie how many itemTypes were deallocated)
        int trim();

        //does NOT resize the vector
        //merely increases the size of the
        // underlying array, so that future
        // growing will be faster
        void ensureCapacity(int c);

    private:
        int mySize;
        int myCapacity;
        itemType *myItems;
};

#include "../MBVector.cpp"

#endif //MBVECTOR_HPP_201001091353
