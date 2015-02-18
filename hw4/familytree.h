#ifndef FAMILYTREE_H
#define	FAMILYTREE_H

#include "familyRunner.h"

class FamilyTree {
  
public:
  FamilyTree(Family *families, int familyCount);
  void runQueries(Query *queries, Person *answers, int queryCount);


};

#endif	/* FAMILYTREE_H */

