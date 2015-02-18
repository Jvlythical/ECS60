#include <iostream>
#include <climits>

#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

void LeafNode::addToLeft(int value, int last)
{
  leftSibling->insert(values[0]);

  for(int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  values[count - 1] = last;
  if(parent)
    parent->resetMinimum(this);
} // LeafNode::ToLeft()

void LeafNode::addToRight(int value, int last)
{
  rightSibling->insert(last);

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToRight()

void LeafNode::addToThis(int value)
{
  int i;

  for(i = count - 1; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];

  values[i + 1] = value;
  count++;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToThis()


void LeafNode::addValue(int value, int &last)
{
  int i;

  if(value > values[count - 1])
    last = value;
  else
  {
    last = values[count - 1];

    for(i = count - 2; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];
    // i may end up at -1
    values[i + 1] = value;
  }
} // LeafNode:: addValue()


int LeafNode::getMaximum()const
{
  if(count > 0)  // should always be the case
    return values[count - 1];
  else
    return INT_MAX;
} // getMaximum()


int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  int last;

  if(count < leafSize)
  {
    addToThis(value);
    return NULL;
  } // if room for value

  addValue(value, last);

  if(leftSibling && leftSibling->getCount() < leafSize)
  {
    addToLeft(value, last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < leafSize)
    {
      addToRight(value, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(value, last);
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

/* ------------------ Added methods -------------------- */

LeafNode* LeafNode::remove(int value)
{   // To be written by students
  bool found = false;
	
	for(int i=0; i < count; i++){
		if (values[i] == value) {
			if(count != 1) values[i] = values[i+1];
			found = true;
		}

		if(values[i] > value && found)values[i] = values[i+1];
  }

	if(found) count--;

//Start borrowing checking

  return (LeafNode*) tryBorrowing();  // filler for stub
}  // LeafNode::remove()

BTreeNode* LeafNode::tryBorrowing() {

	if(!this->isHalfFull()) {
		bool has_borrowed = false;
		BTreeNode *left_leaf = this->getLeftSibling();
		BTreeNode *right_leaf =  this->getRightSibling();

		if(left_leaf != NULL) {
			if(left_leaf->canBorrow()) {
				((LeafNode *) left_leaf)->moveChildRight();
				return this;
			}
		} 
		
		if(right_leaf != NULL && !has_borrowed) {
			if (right_leaf->canBorrow()) {
				((LeafNode *) right_leaf)->moveChildLeft();
				return this;
			}
		}
	
		if(!has_borrowed) {
			if(left_leaf != NULL) merge(1);
			else if(right_leaf != NULL) merge(0);
		}

	}

	return this;
}

BTreeNode* LeafNode::moveChildLeft() {
	if(this->leftSibling == NULL) return this;
	
	LeafNode *left_leaf = static_cast<LeafNode *>(this->leftSibling);
	left_leaf->insert(values[0]);
	
	for(int i = 0; i < count - 1; i++) values[i] = values[i + 1];

	--count;
	return this;
}

BTreeNode* LeafNode::moveChildRight() {
	if(this->rightSibling == NULL) return this;
	
	//int package = values[count - 1]; 
	//((LeafNode *) this->rightSibling)->insert(package);

	LeafNode *right_leaf = static_cast<LeafNode *>(this->rightSibling);
	right_leaf->insert(values[count - 1]);
	
	--count;
	return this;
}

void LeafNode::showInfo(const char* msg) {
	cout << endl << msg << endl;
	cout << "-------------------------------" << endl;
	cout << "Leaf node values: ";
	
	for(int i = 0; i < count; i++) {
		 cout << values[i] << " ";
	}

	cout << endl << "-------------------------------" << endl << endl;
}

bool LeafNode::canBorrow() {
	if(count > (leafSize / 2) + leafSize % 2) return true;
	else return false;
}

bool LeafNode::isHalfFull() {
	if(count >= (leafSize / 2) + leafSize % 2) return true;
	else return false;
}


/* ----------------------------------------------------- */

LeafNode* LeafNode::split(int value, int last)
{
  LeafNode *ptr = new LeafNode(leafSize, parent, this, rightSibling);


  if(rightSibling)
    rightSibling->setLeftSibling(ptr);

  rightSibling = ptr;

  for(int i = (leafSize + 1) / 2; i < leafSize; i++)
    ptr->values[ptr->count++] = values[i];

  ptr->values[ptr->count++] = last;
  count = (leafSize + 1) / 2;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
  return ptr;
} // LeafNode::split()

