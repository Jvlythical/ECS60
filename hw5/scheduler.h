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
  void printJobsAr();

private:
	int num_jobs;
	int cri_leng;
	int free_ppl;
	int num_ppl;
	
	int *dep_chart;
	int *key_chart;
	int *cri_chart;

	Job* jobs_ar;
	JobWrapper *jl;

	QueueLL<int> ppl_q;
	QueueLL<int> parallel_q;
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
	int driveAssignPeople(QueueLL<JobWrapper>, int, int, int);
	bool assignPeople(int, int);
	bool assignPeople(JobWrapper&, int);
	void freePeople(int);
	void freePeople(const JobWrapper&);

}; // class Scheduler

#endif
