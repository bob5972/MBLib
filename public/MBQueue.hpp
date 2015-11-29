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

#ifndef MBQueue_HPP_201001091433
#define MBQueue_HPP_201001091433

#include "mbtypes.h"
#include "mbassert.h"

template<class itemType>
class MBQueue
{
    public:

// constructors/destructor
        MBQueue()
                : mySize(0), myTop(NULL), myBottom(NULL), myLast(NULL)
        {
        }

        MBQueue(const MBQueue & q);               // copy constructor
        ~MBQueue()
        {
            makeEmpty();
            if (myLast) {
                delete myLast;
            }

            myLast = NULL;
        }

        // assignment

        const MBQueue & operator =(const MBQueue & rhs);

// accessors
        // return true if empty else false
        bool isEmpty() const
        {
            return (mySize == 0);
        }

        //Returns the next element to be dequeued
        const itemType & front() const
        {
            ASSERT(mySize > 0);
            return myTop->value;
        }

        int size() const
        {
            return mySize;
        }

// modifiers
        void makeEmpty();    // make queue empty

        void enqueue(const itemType &item)
        {
            if (mySize > 0) {
                myBottom->nextNode = new MBNode(item, NULL);
                myBottom = myBottom->nextNode;
                mySize++;
            } else {
                myTop = new MBNode(item, NULL);
                myBottom = myTop;
                mySize = 1;
            }
        }

        const itemType& dequeue()
        {
            ASSERT(mySize > 0);

            if (myLast) {
                delete myLast;
            }

            myLast = myTop;
            myTop = myTop->nextNode;
            mySize--;
            return myLast->value;
        }

        void dequeue(itemType &item)
        {
            ASSERT(mySize > 0);

            item = front();
            dequeue();
        }

    private:
        struct MBNode
        {
                MBNode(const itemType&val)
                        : nextNode(NULL), value(val)
                {
                }
                MBNode(const itemType&val, MBNode*target)
                        : nextNode(target), value(val)
                {
                }
                MBNode(const MBNode&old)
                        : nextNode(old.nextNode), value(old.value)
                {
                }
                MBNode()
                        : nextNode(NULL)
                {
                }

                MBNode* nextNode;
                itemType value;
        };

        int mySize;                    // # of elts currently in queue
        MBNode* myTop;
        MBNode* myBottom;
        MBNode* myLast;	//hack to implement itemType& dequeue

};

#include "../MBQueue.cpp"

#endif //MBQueue_HPP_201001091433
