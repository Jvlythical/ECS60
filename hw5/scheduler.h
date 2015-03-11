#ifndef schedulerH
#define schedulerH

#include "ProjectRunner.h"

class Scheduler
{
public:
  Scheduler(int numJobs, int numWorkers, Job *jobs, int numPeople);
  void run();
}; // class Scheduler

#endif
