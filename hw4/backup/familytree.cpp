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
	int keys[100000], count = 0;

	edges = new QuadraticHashTable<PersonWrapper> (pw, familyCount * 7 * 2);
	
	for(int i = 0; i < familyCount; i++) {
		Family family = families[i];
		
		pw.uid = createUid(family.person);
		if(edges->find(pw.uid).uid <=  0) {
			pw.person = family.person;
			edges->insert(pw);

			keys[count] = pw.uid;
			count++;
		}

		pw.uid = createUid(family.spouse);
		if(edges->find(pw.uid).uid <= 0) {
			pw.person = family.spouse;
			edges->insert(pw.uid, pw);

			keys[count] = pw.uid;
			count++;
		}

		for(int n = 0; n < family.childCount; n++) {
			pw.uid = createUid(family.children[n]);
			pw.person = family.children[n];
			pw.parent[0] = family.person;
			pw.parent[1] = family.spouse;
			edges->insert(pw.uid, pw);

			keys[count] = pw.uid;
			count++;
		}
	}

	//for(int i = 0; i < count; i++) {
		//cout << i << ": " << edges->find(keys[i]).person.firstName << " " << edges->find(keys[i]).person.lastName << " " << keys[i] << endl;
	//}


} // FamilyTree()

void FamilyTree::runQueries(Query *queries, Person *answers, int queryCount)
{

	PersonWrapper pw_tmp;
	pw_tmp.person.year = -1;
	QuadraticHashTable<PersonWrapper> *ht = new QuadraticHashTable<PersonWrapper>(pw_tmp, 100000);
	
	for(int i = 0; i < queryCount; i++) {
		PersonWrapper pw = edges->find(createUid(queries[i].person1));
		PersonWrapper pw_tmp = pw;
		pw_tmp.person.year = -1;

		QuadraticHashTable<PersonWrapper> *ansc1 = extractAncestors(pw, ht);


		pw = edges->find(createUid(queries[i].person2));
		pw = findYoungest(ansc1, pw, pw_tmp);

		ht->makeEmpty();
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

	cout << "Ancestor: " << pw.person.firstName << endl;
	ht->insert(pw.uid, pw);
	extractAncestors(edges->find(createUid(pw.parent[0])), ht);
	extractAncestors(edges->find(createUid(pw.parent[1])), ht);

	return ht;
}

PersonWrapper FamilyTree::findYoungest(QuadraticHashTable<PersonWrapper>* ht, PersonWrapper pw, PersonWrapper youngest) {
	if(pw.uid <= 0) return youngest;

//	cout << pw.person.firstName << endl;
	if(pw.person.year > youngest.person.year) youngest = pw;
	findYoungest(ht, ht->find(createUid(pw.parent[0])), youngest);
	findYoungest(ht, ht->find(createUid(pw.parent[1])), youngest);
	
	return youngest;
}
