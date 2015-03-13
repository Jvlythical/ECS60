#include <iostream>
#include <cstring>

#include "scheduler.h"
#include "JobWrapper.h"

using namespace std;

Scheduler::Scheduler(int numJobs, int numWorkers, Job *jobs, int numPeople)
{
	num_jobs = numJobs;
	dep_chart = new int[numJobs];
	jl = new JobWrapper[numJobs];

	for(int i = 0; i < numJobs; i++) {
		JobWrapper jw;

		jw.uid = i;
		jw.job = jobs[i];

		jl[i] = jw;
	}
	
	//this->createDepChart(jobs);
	//this->topSort(jobs, NULL);
	//this->queueJobs();
	//this->run();

	for(int i = 0; i < numJobs; i++) {
		cout << endl;
		cout << jobs[i].length << endl;


		for(int n = 0; n < jobs[i].numDependencies; n++) {
			cout << "Dependency: " << jobs[i].dependencies[n] << endl;
		}
	}

	//topSort(jobs, &Scheduler::calcWaveECT);
} // Scheduler()

void Scheduler::run()
{
	// Create a copy of the dependency chart
	int dep_cpy[num_jobs], marker[1000];
	int marker_pos;
	JobWrapper jwt;
	memcpy(dep_cpy, dep_chart, num_jobs * sizeof(int));

	// Begin assigning people
	while(!crit.empty() && !reg.empty()) {

		// Start with the critical jobs
		jwt = crit.front();
		while(dep_cpy[jwt.uid] == 0) {
			assignPeople(&jwt);
			
			// Mark for dependency deletion
			for(int i = 0; i < jwt.job.numDependencies; ++i) {
				marker[marker_pos] = jwt.job.dependency[i];
				marker_pos += 1;
			}

			crit.pop();
			jwt = crit.front();
		} 	
		
		// Look at the regular jobs
		jwt = reg.front();
		while(dep_cpy[jwt.uid] == 0) {
			assignPeople(&jwt);
			
			// Mark for dependency deletion 
			for(int i = 0; i < jwt.job.numDependencies; ++i) {
				marker[marker_pos] = jwt.job.dependency[i];
				marker_pos += 1;
			}

			reg.pop();
			jwt = reg.front();
		}

		// Prepare for next wave
		for(int i = 0; i < marker_pos; i++) 
			--dep_cpy[i];

		marker_pos = 0;
	}

} // run()

void Scheduler::createDepChart(Job *jobs) {

}

void Scheduler::topSort(Job *jobs, TopSortCB callback = NULL) {

		//cout << (this->*callback)(&jw, 0, 0 ) << endl;	
}

void Scheduler::calcWaveST(JobWrapper* wave, int wave_size, int prev_ect) {
	for(int i = 0; i < wave_size; i++) 
		wave[i].st = prev_ect - wave[i].ct;
}

int Scheduler::calcWaveECT(JobWrapper* wave, int wave_size, int prev_ct) {
	int ect = 0;

	for(int i = 0; i < wave_size; i++) {
		wave[i].ct = prev_ct + wave[i].job.length;
		if(wave[i].ct > ect)
			ect = wave[i].ct;
	}

	return ect;
}

void Scheduler::queueJobs() {
	for(int i = 0; i < num_jobs; i++) {
		if(jl[i].st == 0) crit.push(jl[i]);	
		else reg.push(jl[i]);
	}
}

void Scheduler::assignPeople(JobWrapper jw) {

}
