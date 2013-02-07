#ifndef MBStack_HPP_201001091426
#define MBStack_HPP_201001091426

#include "mbtypes.h"
#include "MBVector.hpp"

template<class itemType>
class MBStack
{
  public:

// constructors/destructor
    MBStack( );                                 // construct empty stack
    MBStack( const MBStack & s );               // copy constructor
    ~MBStack( );                                // destructor

// assignment
    const MBStack & operator = ( const MBStack & rhs );

// accessors
	const itemType& top() const;//return top (no pop)
	const itemType& peek() const;

    bool  isEmpty( ) const;      // return true if empty else false
    int   size() const;

// modifiers
    void push( const itemType & item );    // insert item (on top)
    const itemType& pop( );                // remove top element
    void pop (itemType& item);
    void makeEmpty( );                     // make queue empty
    void trim();  // reduce memory usage

	private:
		MBVector<itemType> myVector;
};

#include "../MBStack.cpp"

#endif //MBStack_HPP_201001091426
