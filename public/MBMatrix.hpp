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

#ifndef MBMatrix_HPP_201002202128
#define MBMatrix_HPP_201002202128

#include "MBVector.hpp"

template<class itemType>
class MBMatrix
{
    public:
        MBMatrix();
        MBMatrix(const MBMatrix& m);
        explicit MBMatrix(int dim);
        MBMatrix(int rows, int columns);

        MBMatrix(int rows, int columns, const itemType & fillValue);

        //Destructor
        ~MBMatrix();

        //Assignment
        const MBMatrix & operator =(const MBMatrix & m);

        //Accessors
        int numRows() const;
        int numColumns() const;
        int numCols() const;

        const itemType & get(int row, int col) const;
        itemType& get(int row, int col);

        void put(int row, int col, const itemType& t);

        void fillAll(const itemType& t);

        //Modifiers
        void resize(int newRow, int newCol);
        void resize(int dim);

    private:
        int myRows;
        int myCols;
        MBVector<MBVector<itemType> > myItems;
};

#include "MBMatrix.cpp"
#endif //MBMatrix_HPP_201002202128
