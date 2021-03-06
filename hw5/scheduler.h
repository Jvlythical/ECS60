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
	int free_ppl;
	int num_ppl;
	int max_ppl;
	
	int *dep_chart;
	int *key_chart;
	int *cri_chart;

	JobWrapper *jl;

	QueueLL<int> ppl_q;
	QueueLL<JobWrapper> crit;
	QueueLL<JobWrapper> reg;

	// Initialization methods
  int* createDepChart(Job*);
  void createPeopleQueue(int);
  JobWrapper* wrapJobs(Job*);

	// Set-up methods
	JobWrapper* topSort();
  void calcTimes(JobWrapper*);
 	
 	// Run methods
  void queueJobs();

 	// Helper methods
  bool checkIsDependent(JobWrapper, int);
  JobWrapper calcWaveECT(QueueLL<JobWrapper>, JobWrapper, JobWrapper*);
	void calcWaveST(QueueLL<JobWrapper>, int, JobWrapper*);
	bool assignPeople(JobWrapper&, int);
	void freePeople(const JobWrapper&);

}; // class Scheduler

#endif
