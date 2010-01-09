#ifndef MBTreeSet_CPP_201001091449
#define MBTreeSet_CPP_201001091449

#include<stdlib.h>

#include "MBTreeSet.h"
#include "mjbdebug.h"
#include "mjbassert.h"

template <class itemType>
MBTreeSet<itemType>::MBTreeSet()
:root(NULL),
 mySize(0)
{ }

template<class itemType>
MBTreeSet<itemType>::~MBTreeSet()
{
	makeEmpty();
}

template <class itemType>
void MBTreeSet<itemType>::makeEmpty()
{
	delete root;
	root = NULL;
	mySize = 0;
}

template <class itemType>
bool MBTreeSet<itemType>::isEmpty() const
{
	return mySize > 0;
}

template <class itemType>
void MBTreeSet<itemType>::insert(const itemType& item)
{
	MBTreeNode **curNode = & root;
	while( (*curNode) != NULL) {
		if( (*curNode)->data < item) {
			curNode = &((*curNode)->left);
		} else if( (*curNode)->data > item) {
			curNode = &((*curNode)->right);
		} else if( (*curNode)->data == item) {
			return;
		}
	}
	mySize++;
	*curNode = new MBTreeNode(item,NULL,NULL);
}

template <class itemType>		
void MBTreeSet<itemType>::remove(const itemType& item)
{
	MBTreeNode **curNode = & root;
	MBTreeNode **parent = NULL;
	while( (*curNode) != NULL && (*curNode)->data != item) {
		if( (*curNode)->data < item) {
			curNode = &((*curNode)->left);
		} else if( (*curNode)->data > item) {
			curNode = &((*curNode)->right);
		}
	}
	
	if(*curNode == NULL) {
		return;
		//item is not in the tree
	}
	
	//*curNode points to the node to be deleted
	if((*curNode)->left == NULL && (*curNode)->right == NULL) {
		delete (*curNode);
		(*curNode) = NULL;
		return;
	} else if((*curNode)->left != NULL && (*curNode)->right != NULL) {
		MBTreeNode* badNode = (*curNode);
		MBTreeNode* rTree = badNode->right;
		(*curNode) = badNode->left;
		
		badNode->left = NULL;
		badNode->right = NULL;
		delete badNode;
		badNode = NULL;
		
		//rTree needs to go on.
		
		MBTreeNode**temp = curNode;
		while((*temp) != NULL) {
			temp =&((*temp)->right);
		}
		(*temp) = rTree;
		return;
	} else {
		//1 child;
		MBTreeNode* child = NULL;
		if( (*curNode)->left == NULL) {
			child = (*curNode)->right;
			(*curNode)->right = NULL;
		} else {
			child = (*curNode)->left;
			(*curNode)->left = NULL;
		}
		
		//Both kids already null
		delete (*curNode);
		
		(*curNode) = child;
		return;
	}	
	
}
		
template <class itemType>
bool MBTreeSet<itemType>::contains(const itemType& item) const
{
	MBTreeNode * const*curNode = &root;
	while( (*curNode) != NULL) {
		if( (*curNode)->data < item) {
			curNode = &((*curNode)->left);
		} else if( (*curNode)->data > item) {
			curNode = &((*curNode)->right);
		} else if( (*curNode)->data == item) {
			return true;
		}
	}
	return false;
}
		
template <class itemType>
int MBTreeSet<itemType>::size() const
{
	return mySize;
}

template <class itemType>
MBVector<itemType> MBTreeSet<itemType>::preOrder() const
{
	apvector <itemType> oup(mySize);
	int x=0;
	preOrder(root,oup,x);
	return oup;
}

template <class itemType>
void MBTreeSet<itemType>::preOrder(MBTreeNode *node, MBVector<itemType>&oup, int& index) const
{
	if(node == NULL) {
		return;
	}
	oup[index] = node->data;
	index++;
	preOrder(node->left, oup,index);
	preOrder(node->right,oup,index);
}
	
template <class itemType>
apvector<itemType> MBTreeSet<itemType>::inOrder() const
{
	apvector <itemType> oup(mySize);
	int x=0;
	inOrder(root,oup,x);
	return oup;
}

template <class itemType>
void MBTreeSet<itemType>::inOrder(MBTreeNode *node, MBVector<itemType>&oup, int& index) const
{
	if(node == NULL) {
		return;
	}
	inOrder(node->left, oup,index);
	oup[index] = node->data;
	index++;
	inOrder(node->right,oup,index);
}

template <class itemType>
MBVector<itemType> MBTreeSet<itemType>::postOrder() const
{
	apvector <itemType> oup(mySize);
	int x=0;
	postOrder(root,oup,x);
	return oup;
}

template <class itemType>
void MBTreeSet<itemType>::postOrder(MBTreeNode *node, MBVector<itemType>&oup, int& index) const
{
	if(node == NULL)
	{
		return;
	}
	postOrder(node->left, oup,index);
	postOrder(node->right,oup,index);
	oup[index] = node->data;
	index++;
}
	
	

	
#endif //MBTreeSet_CPP_201001091449
