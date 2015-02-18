
#ifndef FAMILYRUNNER_H
#define FAMILYRUNNER_H

class Person
{
public:
  short year;
  char lastName[12];
  char firstName[12];
  char gender;
}; // class Person

class Family
{
public:
  Person person;
  Person spouse;
  Person children[7];
  short childCount;
};

class Query
{
public:
  Person person1;
  Person person2;
};


#endif

