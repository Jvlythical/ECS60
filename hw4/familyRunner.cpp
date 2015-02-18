#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "familyRunner.h"
#include "CPUTimer.h"
#include "familytree.h"

using namespace std;

int readFamilies(ifstream &inf, Family *families)
{
  char line[1024], *ptr;
  int familyCount = 0, childCount;
  while(inf.getline(line, 1024))
  {
    families[familyCount].person.year = atoi(strtok(line, ","));
    strcpy (families[familyCount].person.lastName, strtok(NULL, ","));
    strcpy (families[familyCount].person.firstName, strtok(NULL, ","));
    families[familyCount].person.gender = *(strtok(NULL,","));
    childCount = 0;
    ptr = strtok(NULL,",");
    
    if(!ptr)
      families[familyCount].spouse.year = -1;
    else
    {
      families[familyCount].spouse.year = atoi(ptr);
      strcpy(families[familyCount].spouse.lastName, strtok(NULL, ",")); // last name
      strcpy(families[familyCount].spouse.firstName, strtok(NULL, ",")); // first name
      families[familyCount].spouse.gender = *strtok(NULL, ","); // gender
      
      while((ptr = strtok(NULL, ",")) != NULL)
      {
        families[familyCount].children[childCount].year = atoi(ptr);
        strcpy(families[familyCount].children[childCount].lastName, strtok(NULL, ","));
        strcpy(families[familyCount].children[childCount].firstName, strtok(NULL, ","));
        families[familyCount].children[childCount++].gender = *(strtok(NULL,","));
      }  // while more children
    }  // if spouse
     families[familyCount++].childCount = childCount;
  }  // while more families in file
  
  return familyCount;
}

void readQueries(ifstream &inf, Query *queries, Person *answerKeys, 
  int queryCount)
{
  char line[1024];
  
  for(int i = 0; i < queryCount; i++)
  {
    inf.getline(line, 1024);
    queries[i].person1.year = atoi(strtok(line, ","));
    strcpy(queries[i].person1.lastName, strtok(NULL, ","));
    strcpy(queries[i].person1.firstName, strtok(NULL, ","));
    queries[i].person1.gender = *(strtok(NULL,","));
    queries[i].person2.year = atoi(strtok(NULL, ","));
    strcpy(queries[i].person2.lastName, strtok(NULL, ","));
    strcpy(queries[i].person2.firstName, strtok(NULL, ","));
    queries[i].person2.gender = *(strtok(NULL,","));
    answerKeys[i].year = atoi(strtok(NULL, ","));
    if(answerKeys[i].year > 0)
    {
      strcpy(answerKeys[i].lastName, strtok(NULL, ","));
      strcpy(answerKeys[i].firstName, strtok(NULL, ","));
      answerKeys[i].gender = *(strtok(NULL,","));
    }
  } // while
}

int main(int argc, char* argv[])
{
  ifstream inf(argv[1]);
  int generations, pairs, queryCount, familyCount;
  char dummy;
  inf >> generations >> dummy >> pairs >> dummy >> queryCount;
  inf.ignore(10, '\n');
  Family *families = new Family[200000];
  Query *queries = new Query[queryCount];
  Person *answers = new Person[queryCount];
  Person *answerKeys = new Person[queryCount];
  readQueries(inf, queries, answerKeys, queryCount);
  familyCount = readFamilies(inf, families);
  CPUTimer ct;
  ct.reset();
  FamilyTree *familyTree = new FamilyTree(families, familyCount);
  delete [] families;
  familyTree->runQueries(queries, answers, queryCount);
  cout << "CPU Time: " << ct.cur_CPUTime() << endl;

  for(int i = 0; i < queryCount; i++)
    if(answerKeys[i].year == -1)
    {
      if(answers[i].year != -1)
      {
        cout << "You found an ancestor when there was none on query #"  << i << endl;
        cout << "Descendent 1: " << queries[i].person1.year << ' ' 
          << queries[i].person1.lastName << ',' << queries[i].person1.firstName  << endl;
        cout << "Descendent 2: " << queries[i].person2.year << ' ' 
          << queries[i].person2.lastName << ',' << queries[i].person2.firstName  << endl;
        cout << "Your answer:" << answers[i].year << ' ' << answers[i].lastName
          << ',' << answers[i].firstName << endl;
      }
    }    
    else  // An ancestor should be found
      if(answers[i].year != answerKeys[i].year 
       || strcmp(answers[i].lastName, answerKeys[i].lastName) != 0
       || strcmp(answers[i].firstName, answerKeys[i].firstName) != 0
       || answers[i].gender != answerKeys[i].gender)
      {
        cout << "Disagreement on query #" << i << endl;
         cout << "Descendent 1: " << queries[i].person1.year << ' ' 
          << queries[i].person1.lastName << ',' << queries[i].person1.firstName  << endl;
        cout << "Descendent 2: " << queries[i].person2.year << ' ' 
          << queries[i].person2.lastName << ',' << queries[i].person2.firstName  << endl;
        cout << "Proper answer: " << answerKeys[i].year << ' ' << answerKeys[i].lastName
          << ',' << answerKeys[i].firstName << endl;
        cout << "Your answer:" << answers[i].year << ' ' << answers[i].lastName
          << ',' << answers[i].firstName << endl;
      }
  return 0;
}  // main()


