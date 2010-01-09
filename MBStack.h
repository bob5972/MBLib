#ifndef MBStack_H_201001091426
#define MBStack_H_201001091426

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

		int mySize;        // # of elts currently in stack
		MBNode* myTop;
		MBNode * myLast;  //Used to store the reference from a call to pop
};

#include "MBStack.cpp"

#endif //MBStack_H_201001091426
