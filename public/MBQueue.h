#ifndef MBQueue_H_201001091433
#define MBQueue_H_201001091433

#include "mbtypes.h"

template<class itemType>
class MBQueue
{
  public:

// constructors/destructor
    MBQueue( );                                 // construct empty queue
    MBQueue( const MBQueue & q );               // copy constructor
    ~MBQueue( );                                // destructor

  // assignment

    const MBQueue & operator = ( const MBQueue & rhs );

// accessors
    bool isEmpty( ) const;        // return true if empty else false
	const itemType & front() const; //Returns the next element to be dequeued
	int size() const;		//returns the size

// modifiers
    void makeEmpty( );    // make queue empty
    const itemType & dequeue();	//takes next item
    void dequeue(itemType &);
    void enqueue(const itemType&);//enqueues item

   private:
		struct MBNode
		{
			MBNode(const itemType&val)
			:nextNode(NULL),
			 value(val) {}
			MBNode(const itemType&val, MBNode*target)
			:nextNode(target),
			 value(val) {}
			MBNode(const MBNode&old)
			:nextNode(old.nextNode),
			 value(old.value) {}
			MBNode()
			:nextNode(NULL) {}
	
			MBNode* nextNode;
			itemType value;
		};
   	   
		int mySize;                    // # of elts currently in queue
		MBNode* myTop;
		MBNode* myBottom;
		MBNode* myLast;	//hack to implement itemType& dequeue
	
};

#include "MBQueue.cpp"

#endif //MBQueue_H_201001091433
