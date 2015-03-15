#ifndef schedulerH
#define schedulerH

#include <queue>

#include "ProjectRunner.h"
#include "JobWrapper.h"


class Scheduler
{
public:
  Scheduler(int numJobs, int numWorkers, Job *jobs, int numPeople);
  void run();
  
  // Debug methods
  void printDepChart();
  void printKeyChart();
  void printJl(JobWrapper *, int);
  void testRun();
  void printQueue(std::queue<JobWrapper>);
  bool checkIsDependent(JobWrapper, int);

private:
	int num_jobs;
	int *dep_chart;
	int *key_chart;
	JobWrapper *jl;
	std::queue<JobWrapper> crit;
	std::queue<JobWrapper> reg;
	
	// Initialization methods
  int* createDepChart(Job*);
  JobWrapper* wrapJobs(Job*);

	// Set-up methods
	JobWrapper* topSort();
	void calcWaveST(std::queue<JobWrapper>, int, JobWrapper*);
  JobWrapper calcWaveECT(std::queue<JobWrapper>, JobWrapper, JobWrapper*);
  void calcTimes(JobWrapper*);
 
 /*
  void calcECT(JobWrapper, int, int &);
  void calcST(JobWrapper, int);
*/
	
	// Run methods
  void queueJobs();
  void assignPeople(JobWrapper &);

}; // class Scheduler

#endif
