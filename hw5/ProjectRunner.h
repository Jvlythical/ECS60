#ifndef PROJECTRUNNER_H
  #define PROJECTRUNNER_H

typedef struct
{
  short length;
  short numDependencies;
  int startTime;
  int finishTime;
  short numPeopleUsed;
  int dependencies[50];
  short peopleIDs[50];
} Job;

#endif

