/*
 * MBSet.hpp -- part of MBLib
 *
 * Copyright (c) 2015-2021 Michael Banack <github@banack.net>
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

#ifndef MBSET_HPP_201001091440
#define MBSET_HPP_201001091440

#include "MBVector.hpp"
#include "BitVector.hpp"

// This class is embarrassingly slow.

template<class itemType>
class MBSet
{
    public:
        MBSet();
        MBSet(const MBSet &);

        ~MBSet();

        const MBSet & operator =(const MBSet &rhs);

        //Accessors
        void makeEmpty();
        bool isEmpty() const;
        int size() const;
        bool contains(const itemType &) const;
        bool add(const itemType &);
        bool insert(const itemType &);
        bool remove(const itemType &);
        const MBVector<itemType>& items() const;

    private:
        MBVector<itemType> myItems;
        BitVector isUsed;

        int mySize;
};

#include "../MBSet.cpp"
#endif // MBSET_HPP_201001091440
