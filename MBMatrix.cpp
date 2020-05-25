/*
 * MBMatrix.cpp -- part of MBLib
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

#ifndef MBMatrix_CPP_201002052320
#define MBMatrix_CPP_201002052320

#include "MBMatrix.hpp"
#include "mbtypes.h"
#include "mbdebug.h"
#include "mbassert.h"

template<class itemType>
MBMatrix<itemType>::MBMatrix()
:myRows(0),
 myCols(0),
 myItems(0)
{}

template<class itemType>
MBMatrix<itemType>::MBMatrix(int dim)
:myRows(0),
 myCols(0),
 myItems(0)
{
	resize(dim,dim);
}

template<class itemType>
MBMatrix<itemType>::MBMatrix(const MBMatrix<itemType>& vec)
:myRows(0),
 myCols(0),
 myItems(0)
{
	resize(vec.myRows,vec.myCols);

	for(int r=0;r<myRows;r++) {
		for(int c=0;c<myCols;c++) {
			myItems[r][c] = vec.myItems[r][c];
		}
	}
}

template<class itemType>
MBMatrix<itemType>::MBMatrix(int rows, int columns, const itemType & fillValue)
:myRows(0),
 myCols(0),
 myItems(0)
{
	resize(rows,columns);

	fillAll(fillValue);
}

template<class itemType>
MBMatrix<itemType>::~MBMatrix()
{

}

template <class itemType>
const MBMatrix<itemType> &
	MBMatrix<itemType>::operator = (const MBMatrix<itemType> & rhs)
{
	if (this != &rhs) {
		resize(rhs.myRows, rhs.myCols);
		for (int r=0;r<myRows;r++) {
			for(int c=0;c<myCols;c++) {
				myItems[r][c] = rhs.myItems[r][c];
			}
		}
	}
	return *this;
}


template<class itemType>
int MBMatrix<itemType>::numRows() const
{
	return myRows;
}

template<class itemType>
int MBMatrix<itemType>::numColumns() const
{
	return myCols;
}

template<class itemType>
int MBMatrix<itemType>::numCols() const
{
	return myCols;
}

template <class itemType>
itemType & MBMatrix<itemType>::get (int r, int c)
{
	if (r >= myRows || r < 0) {
		PANIC("Illegal Matrix row.");
	}

	if (c >= myCols || c < 0) {
		PANIC("Illegal Matrix column.");
	}

	return myItems[r][c];
}

template <class itemType>
const itemType & MBMatrix<itemType>::get (int r, int c) const
{
	if (r >= myRows || r < 0) {
		PANIC("Illegal Matrix row.");
	}

	if (c >= myCols || c < 0) {
		PANIC("Illegal Matrix column.");
	}

	return myItems[r][c];
}

template <class itemType>
void MBMatrix<itemType>::put (int r, int c, const itemType& t)
{
	if (r >= myRows || r < 0) {
		PANIC("Illegal Matrix row.");
	}

	if (c >= myCols || c < 0) {
		PANIC("Illegal Matrix column.");
	}

	myItems[r][c] = t;
}

template<class itemType>
void MBMatrix<itemType>::resize(int dim)
{
	resize(dim,dim);
}

template<class itemType>
void MBMatrix<itemType>::resize(int rows, int cols)
{
	if (rows < 0) {
		PANIC("Illegal Matrix row-size.");
	}

	if(cols < 0 ) {
		PANIC("Illegal Matrix col-size.");
	}

	myItems.resize(rows);
	for(int r=0;r<rows;r++) {
		myItems[r].resize(cols);
	}

	myRows = rows;
	myCols = cols;
}

template<class itemType>
void MBMatrix<itemType>::fillAll(const itemType& t)
{
	for(int r=0;r<myRows;r++) {
		for(int c=0;c<myCols;c++) {
			myItems[r][c] = t;
		}
	}
}

#endif //MBMatrix_CPP_201002052320
