// Michael Yen, Victor Wu

#include <iostream>
#include <climits>
#include "InternalNode.h"
#include "LeafNode.h"
using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


BTreeNode* InternalNode::addPtr(BTreeNode *ptr, int pos)
{ // called when original was full, pos is where the node belongs.
  if(pos == internalSize)
    return ptr;

  BTreeNode *last = children[count - 1];

  for(int i = count - 2; i >= pos; i--)
  {
    children[i + 1] = children[i];
    keys[i + 1] = keys[i];
  } // shift things to right to make room for ptr, i can be -1!

  children[pos] = ptr;  // i will end up being the position that it is inserted
  keys[pos] = ptr->getMinimum();
  ptr->setParent(this);
  return last;
} // InternalNode:: addPtr()


void InternalNode::addToLeft(BTreeNode *last)
{
  ((InternalNode*)leftSibling)->insert(children[0]);

  for(int i = 0; i < count - 1; i++)
  {
    children[i] = children[i + 1];
    keys[i] = keys[i + 1];
  }

  children[count - 1] = last;
  keys[count - 1] = last->getMinimum();
  last->setParent(this);
  if(parent)
    parent->resetMinimum(this);
} // InternalNode::ToLeft()

;
void InternalNode::addToRight(BTreeNode *ptr, BTreeNode *last)
{
  ((InternalNode*) rightSibling)->insert(last);
  if(ptr == children[0] && parent)
    parent->resetMinimum(this);
} // InternalNode::addToRight()



void InternalNode::addToThis(BTreeNode *ptr, int pos)
{  // pos is where the ptr should go, guaranteed count < internalSize at start
  int i;

  for(i = count - 1; i >= pos; i--)
  {
      children[i + 1] = children[i];
      keys[i + 1] = keys[i];
  } // shift to the right to make room at pos

  children[pos] = ptr;
  keys[pos] = ptr->getMinimum();
  count++;
  ptr->setParent(this);

  if(pos == 0 && parent)
    parent->resetMinimum(this);
} // InternalNode::addToThis()



int InternalNode::getMaximum() const
{
  if(count > 0) // should always be the case
    return children[count - 1]->getMaximum();
  else
    return INT_MAX;
}  // getMaximum();


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{  // count must always be >= 2 for an internal node
  int pos; // will be where value belongs

  for(pos = count - 1; pos > 0 && keys[pos] > value; pos--);

  BTreeNode *last, *ptr = children[pos]->insert(value);
  if(!ptr)  // child had room.
    return NULL;

  if(count < internalSize)
  {
    addToThis(ptr, pos + 1);
    return NULL;
  } // if room for value

  last = addPtr(ptr, pos + 1);

  if(leftSibling && leftSibling->getCount() < internalSize)
  {
    addToLeft(last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < internalSize)
    {
      addToRight(ptr, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(last);
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  children[0] = oldRoot;
  children[1] = node2;
  keys[0] = oldRoot->getMinimum();
  keys[1] = node2->getMinimum();
  count = 2;
  children[0]->setLeftSibling(NULL);
  children[0]->setRightSibling(children[1]);
  children[1]->setLeftSibling(children[0]);
  children[1]->setRightSibling(NULL);
  oldRoot->setParent(this);
  node2->setParent(this);
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *newNode)
{ // called by sibling so either at beginning or end
  int pos;

  if(newNode->getMinimum() <= keys[0]) // from left sibling
    pos = 0;
  else // from right sibling
    pos = count;

  addToThis(newNode, pos);
} // InternalNode::insert(BTreeNode *newNode)L

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


/* ------------------ Added methods -------------------- */

BTreeNode* InternalNode::remove(int value) {  
	BTreeNode *leaf = NULL, *node = NULL;

  for(int i = 0; i < count; i++) {
  	
  	if(i == 0 && value <= keys[i]) leaf = children[0];
  	if(i == count - 1 && value >= keys[i]) leaf = children[i];
  	if(value >= keys[i] && value < keys[i+1]) leaf = children[i];

		if(leaf != NULL) {
			leaf = leaf->remove(value);
			int node_count = count;
				//keys[i]	= children[i]->getMinimum();

			for(int i = 0; i < node_count; i++) {
				if(children[i]->getCount() == 0) {
					BTreeNode* left_node = children[i]->getLeftSibling();
					BTreeNode* right_node = children[i]->getRightSibling();

					if(left_node != NULL) left_node->setRightSibling(right_node);
					if(right_node != NULL)right_node->setLeftSibling(left_node);

					for(; i < count - 1; i++) {
						children[i] = children[i + 1];
						keys[i] = keys[i + 1];
					}

					--count;
					break;
				}
			}
		}

	}

	node = this->tryBorrowing(); 
	this->prune();

	if(parent == NULL && this->count <= 1) {
		return this->children[0];
	}
	else return this;
} // InternalNode::remove(int value)

BTreeNode* InternalNode::tryBorrowing() {
 	if(!this->isHalfFull()) {
		bool has_borrowed = false;	
		InternalNode *left_node = (InternalNode *) this->leftSibling;
		InternalNode *right_node =  (InternalNode *) this->rightSibling;

		if(left_node != NULL) {
			if(left_node->canBorrow()) {
				left_node->moveChildRight();
				has_borrowed = true;
			}
		} 
		
		if(right_node != NULL && !has_borrowed) {
			if(right_node->canBorrow()) {
				right_node->moveChildLeft();
				has_borrowed = true;
			}
		}

		if(!has_borrowed) {
			if(this->getLeftSibling() != NULL) this->merge(1);
			else if(this->getRightSibling() != NULL) this->merge(0);
		}

		int node_count = count;

	// Drive the trickle down the tree
		for(int i = 0; i < node_count; i++) {
			InternalNode* child = dynamic_cast<InternalNode *> (this->children[i]); 
			if(child != NULL) child->tryBorrowing();
		
		}

 	}

	return this;
}


BTreeNode* InternalNode::merge(bool direction) {
	BTreeNode* new_node = NULL;
	int node_count = count;	
	// 1 = Merge left
	// 0 = Merge right

	if(direction) {
		
		for(int i = 0; i < node_count; i++) {
			this->moveChildLeft();
		}

		new_node = this->leftSibling;
		this->leftSibling->setRightSibling(this->rightSibling);
	} else {

		for(int i = 0; i < node_count; i++) {
			this->moveChildRight();
		}

		new_node = this->rightSibling;
		new_node->setLeftSibling(this->leftSibling);

	}

	return new_node;
}

BTreeNode* InternalNode::moveChildLeft() {
	if(this->leftSibling == NULL) return this;
	
	InternalNode *left_node = static_cast<InternalNode *> (this->leftSibling);

	// Move smallest valued element to left_node
	left_node->insert(children[0]);
	
	// Shift remaining nodes left
	for(int i = 0; i < count - 1; i++) {
		children[i] = children[i + 1];
    keys[i] = keys[i + 1];
	}

	--count;
	return this;
}

BTreeNode* InternalNode::moveChildRight() {
	if(this->rightSibling == NULL) return this;
	
	InternalNode *right_node = static_cast<InternalNode *> (this->rightSibling);
	right_node->insert(children[count - 1]);
	
	//children[count - 1] = NULL;
	//keys[count - 1] = NULL;

	--count;
	return this;
}

bool InternalNode::canBorrow() {
	if(count > (internalSize / 2) + internalSize % 2) return true;
	else return false;
}

bool InternalNode::isHalfFull() {
	if(count >= (internalSize / 2) + internalSize % 2) return true;
	else return false;
}

void InternalNode::prune() {
	int node_count = count;

	for(int i = 0; i < node_count; i++) {
		keys[i] = children[i]->getMinimum();

		if(children[i]->getCount() == 0) {
			for(; i < count; i++) keys[i] = keys[i + 1];

		// Shift children    
			for(; i < node_count - 1; i++) {
				children[i] = children[i + 1];
				keys[i] = children[i]->getMinimum();
			}

	
			break;
		}
	}

}

/* --------------------------------------------------- */

void InternalNode::resetMinimum(const BTreeNode* child)
{
  for(int i = 0; i < count; i++)
    if(children[i] == child)
    {
      keys[i] = children[i]->getMinimum();
      if(i == 0 && parent)
        parent->resetMinimum(this);
      break;
    }
} // InternalNode::resetMinimum()


InternalNode* InternalNode::split(BTreeNode *last)
{
  InternalNode *newptr = new InternalNode(internalSize, leafSize,
    parent, this, rightSibling);


  if(rightSibling)
    rightSibling->setLeftSibling(newptr);

  rightSibling = newptr;

  for(int i = (internalSize + 1) / 2; i < internalSize; i++)
  {
    newptr->children[newptr->count] = children[i];
    newptr->keys[newptr->count++] = keys[i];
    children[i]->setParent(newptr);
  }

  newptr->children[newptr->count] = last;
  newptr->keys[newptr->count++] = last->getMinimum();
  last->setParent(newptr);
  count = (internalSize + 1) / 2;
  return newptr;
} // split()

