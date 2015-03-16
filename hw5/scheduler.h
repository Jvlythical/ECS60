#ifndef schedulerH
#define schedulerH

#include <queue>

#include "ProjectRunner.h"
#include "JobWrapper.h"
#include "QueueLL.h"

class Scheduler
{
public:
  Scheduler(int numJobs, int numWorkers, Job *jobs, int numPeople);
  void run();
  ~Scheduler();
  
  // Debug methods
  void printDepChart();
  void printKeyChart();
  void printJl(JobWrapper *, int);
  void printQueue(QueueLL<JobWrapper>);
  void printCriChart();

private:
	int num_jobs;
	int cri_leng;
	int *dep_chart;
	int *key_chart;
	int *cri_chart;
	JobWrapper *jl;
	QueueLL<JobWrapper> crit;
	QueueLL<JobWrapper> reg;
	
	// Initialization methods
  int* createDepChart(Job*);
  JobWrapper* wrapJobs(Job*);

	// Set-up methods
	JobWrapper* topSort();
  void calcTimes(JobWrapper*);
 	
 	// Run methods
  void queueJobs();
  void assignPeople(JobWrapper &);

 	// Helper methods
  bool checkIsDependent(JobWrapper, int);
  JobWrapper calcWaveECT(QueueLL<JobWrapper>, JobWrapper, JobWrapper*);
	void calcWaveST(QueueLL<JobWrapper>, int, JobWrapper*);

}; // class Scheduler

#endif
