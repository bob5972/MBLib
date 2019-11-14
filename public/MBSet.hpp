/*
 * This file is part of MBLib.
 * Copyright (c) 2015-2019 Michael Banack <bob5972@banack.net>
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
