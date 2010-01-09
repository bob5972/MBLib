#ifndef MBListNode_CPP_201001091424
#define MBListNode_CPP_201001091424

#include "MBListNode.h"

template<class itemType>
MBListNode<itemType>::MBListNode(const itemType &newValue)
:	next(NULL),
	value(newValue)
{
}

template<class itemType>
MBListNode<itemType>::MBListNode(const itemType &newValue, MBNode<itemType> *targetNode)
:	next(targetNode),
	value(newValue)
{
}

template<class itemType>
MBListNode<itemType>::MBListNode()
:	nextNode(NULL)
{
}



#endif //MBListNode_CPP_201001091424
