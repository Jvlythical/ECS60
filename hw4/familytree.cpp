#include <iostream>
#include <cstdlib>
#include <cstring>

#include "familytree.h"

#include "PersonWrapper.h"
#include "HashTable.h"
#include "StackAr.h"

using namespace std;

FamilyTree::FamilyTree(Family *families, int familyCount)
{
	PersonWrapper pw;
	pw.uid = -1;

	edges = new QuadraticHashTable<PersonWrapper> (pw, familyCount * 7);

	for(int i = 0; i < familyCount; i++) {
		Family family = families[i];
		Person null_parent;
		null_parent.firstName[0] = '\0';
		null_parent.lastName[0] = '\0';
		null_parent.year = 0;

		int p1_uid = createUid(family.person);
		pw.uid = p1_uid;
		if(edges->find(family.person).uid <=  0) {
			pw.person = family.person;

			pw.p1_uid = 0;
			pw.p2_uid = 0;

			pw.parent[0] = null_parent;
			pw.parent[1] = null_parent;
			edges->insert(pw);

			people++;
		}

		int p2_uid = createUid(family.spouse);
		pw.uid = p2_uid;
		if(edges->find(family.person).uid <= 0 && family.spouse.year != -1) {
			pw.person = family.spouse;

			pw.p1_uid = 0;
			pw.p2_uid = 0;

			pw.parent[0] = null_parent;
			pw.parent[1] = null_parent;
			edges->insert( pw);

			people++;
		}

		for(int n = 0; n < family.childCount; n++) {
			pw.uid = createUid(family.children[n]);

			pw.p1_uid = p1_uid;
			pw.p2_uid = p2_uid;

			pw.person = family.children[n];
			pw.parent[0] = family.person;
			pw.parent[1] = family.spouse;
			edges->insert( pw);

			people++;
		}
	}
} // FamilyTree()


/* =================
 *   Run queries
 * =================
 */

void FamilyTree::runQueries(Query *queries, Person *answers, int queryCount)
{

	PersonWrapper pw_tmp;
	pw_tmp.person.year = -1;
	
	for(int i = 0; i < queryCount; i++) {

		QuadraticHashTable<PersonWrapper>ht(pw_tmp, 1250);
		PersonWrapper pw = edges->find(queries[i].person1);
		
		// Get ancestors for person 1
		extractAncestors(pw, &ht);
		
		// Find youngest ancestor
		pw = edges->find(queries[i].person2);
		pw_tmp.person.year = -1;
		answers[i] = findYoungest(&ht, pw, pw_tmp, 0).person;

	}

}  // runQueries()

/* =========================
 *   Generate unique id
 * =========================
 */

int FamilyTree::createUid(Person p) {
	int seed = 131, uid = 0;
for(unsigned int i = 0; i < strlen(p.firstName); i++) uid = uid * seed + p.firstName[i];
for(unsigned int i = 0; i < strlen(p.lastName); i++) uid = uid * seed + p.lastName[i];
uid = uid * seed + p.year;
uid = uid * seed + p.gender;	
	if(uid < 0) uid = -uid;
	return uid;
}

QuadraticHashTable<PersonWrapper>* FamilyTree::extractAncestors(PersonWrapper pw, QuadraticHashTable<PersonWrapper> *ht) {
	if(pw.uid <= 0) return ht;

	PersonWrapper p1 = edges->find(pw.p1_uid);
	PersonWrapper p2 = edges->find(pw.p2_uid);

	ht->insert(pw);

	extractAncestors(p1, ht);
	extractAncestors(p2, ht);
		
	return ht;
}

PersonWrapper FamilyTree::findYoungest(QuadraticHashTable<PersonWrapper>* ht, PersonWrapper pw, PersonWrapper &youngest, int count) {
	if(pw.uid <= 0) return youngest;
	if(youngest.person.year > 0 && pw.person.year < youngest.person.year) return youngest;
	if(ht->find(pw.uid).uid && pw.person.year > youngest.person.year) youngest = pw;

	PersonWrapper p1 = edges->find(pw.p1_uid);
	PersonWrapper p2 = edges->find(pw.p2_uid);

	if(p1.person.year > youngest.person.year) findYoungest(ht, p1, youngest, count);
	if(p2.person.year > youngest.person.year)findYoungest(ht, p2, youngest, count);
	

	return youngest;
}
