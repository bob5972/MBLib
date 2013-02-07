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
