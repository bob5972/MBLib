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
