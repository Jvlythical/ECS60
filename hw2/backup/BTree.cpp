#include <iostream>
#include "BTree.h"
#include "BTreeNode.h"
#include "LeafNode.h"
#include "InternalNode.h"
using namespace std;

BTree::BTree(int ISize, int LSize):internalSize(ISize), leafSize(LSize)
{
  root = new LeafNode(LSize, NULL, NULL, NULL);
} // BTree::BTree()


void BTree::insert(const int value)
{
  BTreeNode *ptr = root->insert(value);
  if(ptr) // root split
  {
    InternalNode *IPtr = new InternalNode(internalSize, leafSize,
      NULL, NULL, NULL);
    IPtr->insert(root, ptr);
    root = IPtr;
  } // if root split
} // BTree::insert()


void BTree::print()
{
  BTreeNode *BTreeNodePtr;
  Queue<BTreeNode*> queue(1000);

  queue.enqueue(root);
  while(!queue.isEmpty())
  {
    BTreeNodePtr = queue.dequeue();
    BTreeNodePtr->print(queue);
  } // while
} // BTree::print()


void BTree::remove(int value)
{  // To be written by students
	root = root->remove(value);
	InternalNode* ptr = dynamic_cast<InternalNode*>(root);
	
	while(root->getCount() <= 1 && ptr != NULL) {
		if(ptr != NULL) root = ptr->getChildren()[0];
		ptr = dynamic_cast<InternalNode*>(root);
	}

} // BTree::remove()
