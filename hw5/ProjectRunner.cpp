#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <map>
#include <cmath>
#include "ProjectRunner.h"
#include "scheduler.h"
#include "CPUTimer.h"

typedef map<short, int> PeopleMap;

using namespace std;

class JobStart{
public:
  JobStart(int st, int ID) : startTime(st), jobID(ID){}
  int startTime;
  int jobID;
  bool operator< (const JobStart &rhs) const {return startTime < rhs.startTime;}
};


void check(int numJobs, int numWorkers, Job *jobs, Job *jobs2, int numPeople)
{
  int ID, *earliestStarts = new int[numJobs], finishTime, maxFinishTime = 0;
  short *ancestors = new short[numJobs];
  memset(earliestStarts, 0, sizeof(int) * numJobs);
  memset(ancestors, 0, sizeof(short) * numJobs);

  multiset<JobStart> jobStarts;
  PeopleMap people;
  PeopleMap::iterator itr2;

  for(int i = 0; i < numJobs; i++)
  {
    jobStarts.insert(JobStart(jobs[i].startTime, i));
    for(int j = 0; j < jobs2[i].numDependencies; j++)
      ancestors[jobs2[i].dependencies[j]]++;
  }  // for i

  for(multiset<JobStart>::iterator itr = jobStarts.begin(); itr != jobStarts.end(); itr++)
  {
    ID = itr->jobID;
    if(ancestors[ID] != 0)
      cout << "Job ID# " << ID << " started before all ancestors completed.\n";

    if(earliestStarts[ID] > jobs[ID].startTime)
      cout << "Job ID# " << ID << " started earlier than ancestors finished.\n";

    if(jobs[ID].numPeopleUsed == 0)
      cout << "Job ID# " << ID << " has no people assigned to it.\n";
    else
    {
      finishTime = int (jobs[ID].startTime + ceil(jobs2[ID].length / double(jobs[ID].numPeopleUsed)));
      for(int i = 0; i < jobs[ID].numPeopleUsed; i++)
      {
        itr2 = people.find(jobs[ID].peopleIDs[i]);
        if(itr2 != people.end())
        {
          if(itr2->second > jobs[ID].startTime)
            cout << "Job ID# " << ID << " uses person #" << itr2->first
              << " before they are available\n";
          people.erase(itr2);
        }
        people.insert(PeopleMap::value_type(jobs[ID].peopleIDs[i], finishTime));
      }
    } // else some people used.
    if(finishTime > maxFinishTime)
      maxFinishTime = finishTime;

    for(int i = 0; i < jobs2[ID].numDependencies; i++)
    {
      if(earliestStarts[jobs2[ID].dependencies[i]] < finishTime)
        earliestStarts[jobs2[ID].dependencies[i]] = finishTime;
      ancestors[jobs2[ID].dependencies[i]]--;
    }  // for every dependent
  } // for each  job

  if(people.size() > (unsigned) numPeople)
    cout << "Program used " << people.size() << " people.\n";

  delete [] earliestStarts;
  delete [] ancestors;

  cout << "Finish time: " << maxFinishTime << endl;
} // check()


void readFile(const char* filename, Job *jobs, Job *jobs2,  int numJobs)
{
  int ID = 0, i, numWorkers;
  char s[1024];

  ifstream inf(filename);
  while(inf.getline(s, 1024))
  {
    strtok(s, ",");
    jobs[ID].numPeopleUsed = 0;
    jobs2[ID].length = jobs[ID].length = atoi(strtok(NULL, ","));
    numWorkers = jobs2[ID].numDependencies =jobs[ID].numDependencies
      = atoi(strtok(NULL,","));
    for(i = 0; i < numWorkers; i++)
      jobs2[ID].dependencies[i] = jobs[ID].dependencies[i]
        = atoi(strtok(NULL, ","));
    ID++;
  } // while more jobs to read

} // readFile()

int main(int argc, char* argv[])  // argv[1] = filename,
// argv[2] = number of workers
{
  CPUTimer ct;
  int numPeople = atoi(argv[2]), numJobs, numWorkers;

  sscanf(argv[1], "Jobs-%d-%d", &numJobs, &numWorkers);
  Job *jobs2, *jobs = new Job[numJobs];
  jobs2 = new Job[numJobs];
  readFile(argv[1], jobs, jobs2, numJobs);

  ct.reset();
  Scheduler *scheduler = new Scheduler(numJobs, numWorkers, jobs, numPeople);
  scheduler->run();
  cout << "CPU time: " << ct.cur_CPUTime() << endl;
  check(numJobs, numWorkers, jobs, jobs2, numPeople);

  delete [] jobs;
  delete [] jobs2;
  delete scheduler;
  return 0;
}

