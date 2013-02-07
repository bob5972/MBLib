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

        const MBSet & operator =(const MBSet & set);

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
