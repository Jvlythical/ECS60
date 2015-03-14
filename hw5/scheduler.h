#ifndef schedulerH
#define schedulerH

#include <queue>

#include "ProjectRunner.h"
#include "JobWrapper.h"


class Scheduler
{
public:
	typedef int (Scheduler::*TopSortCB)(JobWrapper*, int, int);

  Scheduler(int numJobs, int numWorkers, Job *jobs, int numPeople);
  void run();
  int* createDepChart(Job*);
  JobWrapper* wrapJobs(Job*);
  JobWrapper* topSort();
  void calcWaveST(JobWrapper*, int, int);
  int calcWaveECT(JobWrapper*, int, int);
  void queueJobs();
  void assignPeople(JobWrapper &);

  // Debug methods
  void printDepChart();
  void printJl();
  void testRun();

private:
	int num_jobs;
	int *dep_chart;
	JobWrapper *jl;
	std::queue<JobWrapper> crit;
	std::queue<JobWrapper> reg;
	
}; // class Scheduler

#endif
