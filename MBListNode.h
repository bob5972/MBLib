#ifndef MBListNode_H_201001091425
#define MBListNode_H_201001091425

#include "mjbtypes.h"

template<class itemType>
struct MBListNode
{
	MBListNode(const itemType&);
	MBListNode(const itemType&, MBListNode<itemType>*);
	MBListNode();
	
	MBListNode<itemType>* next;
	itemType value;
};

#include "MBListNode.cpp"

#endif //MBListNode_H__201001091425
