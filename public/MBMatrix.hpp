/*
 * MBMatrix.hpp -- part of MBLib
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
