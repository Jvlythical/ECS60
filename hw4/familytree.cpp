#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>

#include "familytree.h"
#include "PersonWrapper.h"
#include "hw3/HashTable.h"
#include "hw3/StackAr.h"

using namespace std;

int FamilyTree::testUniq (Person p1) {
	bool test = false;
	if(!test) return false;
	/*
	 *   Test if the person about to be inserted exists
	 *   If it already exists, all the metadata should match
	 *   else theres a conflict with the UID
	 *
	 */

	// TEST START
		PersonWrapper p2 = edges->find(p1);

		// If the person is already in the hash table
		if(p2.uid > 0) {	
			bool print = false;
			bool same_firstName = strcmp(p1.firstName, p2.person.firstName) == 0;
			bool same_lastName = strcmp(p1.lastName, p2.person.lastName) == 0;
			bool same_year = (p1.year == p2.person.year);
			bool same_gender = p1.gender == p2.person.gender;
			
			if(print) {
				cout << "Same first name: " << same_firstName << endl;
				cout << "Same last name: " << same_lastName << endl;
				cout << "Same year: " << same_year << endl;
				cout << "Same gender: "  << same_gender << endl;
			}

			if(same_firstName && same_lastName && same_year && same_gender) {
				//cout << "Inserted same person!" << endl;
			} else {
				cout << "FOUND CONFLICTING UID" << endl;
			}

		} else {
			//cout << createUid(p1) << endl;
		}
		// TEST END
		
		return 1;
}


int testNull(Person p) {
	if(p.year == 0 && strlen(p.firstName) == 0 && strlen(p.lastName) == 0 && p.gender != 'M' && p.gender != 'G') {
		return true;
	} else {
		cout << "NOT NULL" << endl;
		return false;
	}
}













FamilyTree::FamilyTree(Family *families, int familyCount)
{
	PersonWrapper pw;
	pw.uid = 0;
	
	edges = new QuadraticHashTable<PersonWrapper> (pw, familyCount * 7 * 2);

	for(int i = 0; i < familyCount; i++) {
		Family family = families[i];
		//cout << family.spouse.year << endl;


		Person null_parent;
		null_parent.firstName[0] = '\0';
		null_parent.lastName[0] = '\0';
		null_parent.year = 0;
		null_parent.gender = '\0';

		// Test
		testUniq(family.person);
		testUniq(family.spouse);

		pw.uid = createUid(family.person);
		if(edges->find(family.person).uid <=  0) {
			pw.person = family.person;
			pw.parent[0] = null_parent;
			pw.parent[1] = null_parent;
			edges->insert(pw);

			people++;
		}

		pw.uid = createUid(family.spouse) ;
		if(edges->find(family.person).uid <= 0 && family.spouse.year != -1) {
			pw.person = family.spouse;
			pw.parent[0] = null_parent;
			pw.parent[1] = null_parent;
			edges->insert( pw);

			people++;
		}

		for(int n = 0; n < family.childCount; n++) {
			//Test
			testUniq(family.children[n]);
				
			pw.uid = createUid(family.children[n]);
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
	

	//QuadraticHashTable<PersonWrapper> ht(pw_tmp, 50000);

	for(int i = 0; i < queryCount; i++) {

		QuadraticHashTable<PersonWrapper> *ht = new QuadraticHashTable<PersonWrapper>(pw_tmp, 50000);
		PersonWrapper pw = edges->find(queries[i].person1);
		
		// Get ancestors for person 1
		QuadraticHashTable<PersonWrapper> *ansc1 = extractAncestors(pw, ht);
		
		// Find youngest ancestor
		pw = edges->find(queries[i].person2);
		pw_tmp.person.year = -1;
		answers[i] = findYoungest(ansc1, pw, pw_tmp, 0).person;

		delete(ht);
		//ht.makeEmpty();
	}
	
}  // runQueries()

/* =========================
 *   Generate unique id
 * =========================
 */

int FamilyTree::createUid(Person p) {
	/*
	int uid = -1;

	memcpy(&uid, p.firstName, 4);
	uid = uid + p.lastName[0] * 7+ p.lastName[1] + p.lastName[2] * 7 + p.lastName[3]; 
	uid = uid + p.gender * 13;
	uid = uid + p.year * 7;
	
	if(uid < 0) uid = -uid;
	return uid;
	*/

						int seed = 131;
						int uid = 0;
						
          	for(unsigned int i = 0; i < strlen(p.lastName); i++) {
          		uid = (uid * seed) + p.lastName[i];
          	}
						
						for(unsigned int i = 0; i < strlen(p.firstName); i++) {
          		uid = (uid * seed) + p.firstName[i];
          	}
          	int tmp = p.year;

          	for(int i = 0; i < 3; i++) {
          		//uid = uid* seed + (tmp % 10 +'0') ;
          		tmp = (tmp - tmp % 10) / 10;
          	}
          	
          	uid = uid * seed + p.year;
          	uid = uid * seed + p.gender ;


            if( uid < 0 ) uid = -uid;

            return uid ;
}

QuadraticHashTable<PersonWrapper>* FamilyTree::extractAncestors(PersonWrapper pw, QuadraticHashTable<PersonWrapper> *ht) {
	if(pw.uid <= 0) {
		return ht;
	}

	//if(pw.person.year == 0) return ht;

	PersonWrapper p1 = edges->find(pw.parent[0]);
	PersonWrapper p2 = edges->find(pw.parent[1]);

//cout << pw.person.firstName << " " << pw.person.lastName << " " << pw.uid << endl;
//cout << "Hi my name is " << pw.person.firstName << " (" << pw.uid <<") and my parents are " <<
//	pw.parent[0].firstName << " ("<< edges->find(pw.parent[0]).uid<< ") and " << pw.parent[1].firstName << endl;


	ht->insert( pw);
	extractAncestors(p1, ht);
	extractAncestors(p2, ht);

	return ht;
}

/*
QuadraticHashTable<PersonWrapper>* FamilyTree::extractAncestors(PersonWrapper pw, QuadraticHashTable<PersonWrapper> *ht) {
	StackAr<PersonWrapper> s(100000);
	PersonWrapper p1 = edges->find(pw.parent[0]);
	PersonWrapper p2 = edges->find(pw.parent[1]);
	s.push(p2);

	while(!s.isEmpty()) {
		if(pw.uid >= 0)  {
			ht->insert(pw);
			pw = p1;
			s.push(p2);
		} else {
			pw = s.top();
			//s.pop();
		}
	}

	return ht;
}
*/

PersonWrapper FamilyTree::findYoungest(QuadraticHashTable<PersonWrapper>* ht, PersonWrapper pw, PersonWrapper &youngest, int count) {
	if(pw.uid <= 0) {
		return youngest;
	}

	testUniq(pw.person);

	if(ht->find(pw.person).uid > 0)  {
		if(pw.person.year > youngest.person.year) youngest = pw;
	}

	PersonWrapper p1 = edges->find(pw.parent[0]);
	PersonWrapper p2 = edges->find(pw.parent[1]);

	findYoungest(ht, p1, youngest, count);
	findYoungest(ht, p2, youngest, count);
	
	return youngest;
}
