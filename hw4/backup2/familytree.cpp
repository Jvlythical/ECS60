#include <iostream>
#include <cstdlib>
#include <cstring>

#include "familytree.h"
#include "PersonWrapper.h"
#include "hw3/HashTable.h"

using namespace std;

FamilyTree::FamilyTree(Family *families, int familyCount)
{
	PersonWrapper pw;
	pw.uid = -1;

	edges = new QuadraticHashTable<PersonWrapper> (pw, familyCount * 7 * 4);

	for(int i = 0; i < familyCount; i++) {
		Family family = families[i];
		Person null_parent;
		null_parent.firstName[0] = '\0';
		null_parent.lastName[0] = '\0';
		null_parent.year = 0;

		pw.uid = createUid(family.person);
		if(edges->find(family.person).uid <=  0) {
			pw.person = family.person;
			pw.parent[0] = null_parent;
			pw.parent[1] = null_parent;
			edges->insert(pw);

			people++;
		}

		pw.uid = createUid(family.spouse);
		if(edges->find(family.person).uid <= 0) {
			pw.person = family.spouse;
			pw.parent[0] = null_parent;
			pw.parent[1] = null_parent;
			edges->insert( pw);

			people++;
		}

		for(int n = 0; n < family.childCount; n++) {
			pw.uid = createUid(family.children[n]);
			pw.person = family.children[n];
			pw.parent[0] = family.person;
			pw.parent[1] = family.spouse;
			edges->insert( pw);

			people++;
		}
	}

} // FamilyTree()

void FamilyTree::runQueries(Query *queries, Person *answers, int queryCount)
{

	PersonWrapper pw_tmp;
	pw_tmp.person.year = -1;
	
	for(int i = 0; i < queryCount; i++) {
		cout << endl;

		QuadraticHashTable<PersonWrapper> *ht = new QuadraticHashTable<PersonWrapper>(pw_tmp, 5000);
		PersonWrapper pw = edges->find(queries[i].person1);

		QuadraticHashTable<PersonWrapper> *ansc1 = extractAncestors(pw, ht);

		pw = edges->find(queries[i].person2);
		pw_tmp.person.year = -1;
		answers[i] = findYoungest(ansc1, pw, pw_tmp, 0).person;

		//ht->makeEmpty();
		delete(ht);
	}

}  // runQueries()

int FamilyTree::createUid(Person p) {
	int uid = -1;

	memcpy(&uid, p.firstName, 4);
	uid = uid + p.lastName[0] * p.lastName[1] + p.lastName[2] + p.lastName[3]; 

	//uid = uid * p.firstName[0] + p.firstName[1] + p.firstName[2] + p.firstName[3];

	uid = uid + p.gender;
	uid = uid + p.year;
	

	return uid;
}

QuadraticHashTable<PersonWrapper>* FamilyTree::extractAncestors(PersonWrapper pw, QuadraticHashTable<PersonWrapper> *ht) {
	if(pw.uid <= 0) return ht;

	PersonWrapper p1 = edges->find(pw.parent[0]);
	PersonWrapper p2 = edges->find(pw.parent[1]);

	//if(p1.uid <= 0 && p2.uid <= 0) {
		//ht->insert(pw);
		//return ht;
	//}
	//cout << "Person: " << pw.person.firstName << endl;
	//
	//cout << pw.person.firstName << " " << pw.person.lastName << endl;
	
	ht->insert( pw);
	extractAncestors(p1, ht);
	extractAncestors(p2, ht);

	return ht;
}

PersonWrapper FamilyTree::findYoungest(QuadraticHashTable<PersonWrapper>* ht, PersonWrapper pw, PersonWrapper youngest, int count) {
	if(pw.uid <= 0) return youngest;
	if(!ht->find(pw.person).uid && count > 0) return youngest;
	if(pw.person.year > youngest.person.year && count++ > 0) youngest = pw;

	cout << "Hi my name is " << pw.person.firstName << " and my parents are " <<
	pw.parent[0].firstName << " and " << pw.parent[1].firstName << endl;

	youngest = findYoungest(ht, edges->find(pw.parent[0]), youngest, count);
	youngest = findYoungest(ht, edges->find(pw.parent[1]), youngest, count);
	
	return youngest;
}
