#ifndef FAMILYTREE_H
#define	FAMILYTREE_H

#include "familyRunner.h"
#include "PersonWrapper.h"
#include "hw3/HashTable.h"

class FamilyTree {
  
public:
  FamilyTree(Family *families, int familyCount);
  void runQueries(Query *queries, Person *answers, int queryCount);
  int createUid(Person);
  QuadraticHashTable<PersonWrapper>* extractAncestors(PersonWrapper, QuadraticHashTable<PersonWrapper>*);
  PersonWrapper findYoungest(QuadraticHashTable<PersonWrapper>*, PersonWrapper, PersonWrapper);

	QuadraticHashTable<PersonWrapper> *edges;
};

#endif	/* FAMILYTREE_H */

