#ifndef MBMatrix_CPP_201002052320
#define MBMatrix_CPP_201002052320

#include "MBMatrix.h"
#include "mjbtypes.h"
#include "mjbdebug.h"
#include "mjbassert.h"

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
