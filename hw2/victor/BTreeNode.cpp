#include "BTreeNode.h"

#include <iostream>

using namespace std;

BTreeNode::BTreeNode(int LSize, InternalNode *p, BTreeNode *left,
  BTreeNode *right) : count(0), leafSize(LSize), parent(p), leftSibling(left),
  rightSibling(right)
{
} // BTreeNode()

int BTreeNode::getCount() const
{
  return count;
} // getCount()


BTreeNode* BTreeNode::getLeftSibling()
{
  return leftSibling;
} // getLeftSibling()

void BTreeNode::setParent(InternalNode *p)
{
  parent = p;
} // setParent()

BTreeNode* BTreeNode::getRightSibling()
{
  return rightSibling;
} // getRightSibling()

void BTreeNode::setLeftSibling(BTreeNode *left)
{
  leftSibling = left;
} // setLeftSibling()

void BTreeNode::setRightSibling(BTreeNode *right)
{
  rightSibling = right;
} // setLeftSibling()

/* ------------------ Added methods -------------------- */

BTreeNode* BTreeNode::merge(bool direction) {
	BTreeNode* new_node = NULL;
	
	// 1 = Merge left
	// 0 = Merge right

	if(direction) {
		
		for(int i = 0; i < count; i++) {
			this->moveChildLeft();
			new_node = this->leftSibling;
			this->leftSibling->setRightSibling(this->rightSibling);
		}
	} else {

		for(int i = 0; i < count; i++) {
			this->moveChildRight();
			
			new_node = this->rightSibling;
			new_node->setLeftSibling(this->leftSibling);
		}
	}

	return new_node;
}
