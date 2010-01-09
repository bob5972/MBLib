#ifndef MBTREE_H_201001091448
#define MBTREE_H_201001091448

//NOT TESTED

#include "MBVector.h"

template <class itemType>
class MBTreeSet
{
	public:
		MBTreeSet();
		
		void insert(const itemType& item);
		
		void remove(const itemType& item);
		void makeEmpty();
		
		bool isEmpty() const;
		bool contains(const itemType& item) const;
		
		int size() const;
		
		MBVector<itemType> preOrder()  const;
		MBVector<itemType> inOrder()   const;
		MBVector<itemType> postOrder() const;
		
		~MBTreeSet();
	
	private:		
		class MBTreeNode
		{
			public:
				MBTreeNode(const itemType& nitem,MBTreeNode* nleft=NULL, MBTreeNode* nright=NULL)
				:left(nleft),
				 right(nright),
				 data(nitem)
				{	}
				MBTreeNode* left;
				MBTreeNode* right;
				itemType data;
				
				~MBTreeNode()
				{
					if(left != NULL)
					{
						delete left;
						left = NULL;
					}
						if(right != NULL)
					{
						delete right;
						right = NULL;
					}
				}
		};
		
		void preOrder (MBTreeNode *node, apvector<itemType>&oup, int& index) const;
		void inOrder  (MBTreeNode *node, apvector<itemType>&oup, int& index) const;
		void postOrder(MBTreeNode *node, apvector<itemType>&oup, int& index) const;		

		MBTreeNode* root;
		int mySize;
};

#include "MBTreeSet.cpp"

#endif //MBTREE_H_201001091448

