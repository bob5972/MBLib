/*
 * MBStack.hpp -- part of MBLib
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
