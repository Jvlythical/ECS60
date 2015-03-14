#include <iostream>
#include <cstring>

#include "scheduler.h"
#include "JobWrapper.h"

using namespace std;

Scheduler::Scheduler(int numJobs, int numWorkers, Job *jobs, int numPeople)
{
	num_jobs = numJobs;
	jl = wrapJobs(jobs);
	dep_chart = createDepChart(jobs);
	this->printDepChart();
	this->printJl();
	jl = topSort();
	this->printJl();
	//this->topSort(jobs, NULL);
	//this->queueJobs();
	
	//this->testRun();
	//this->run();

	//topSort(jobs, &Scheduler::calcWaveECT);
} // Scheduler()

/* -------------------
 *   Init methods
 * -------------------
 */

int* Scheduler::createDepChart(Job *jobs) {
	int *tmp = new int[num_jobs];

	for(int i = 0; i < num_jobs; i++) 
		tmp[i] = 0;	

	for(int i = 0; i < num_jobs; i++) {
		for(int n = 0; n < jobs[i].numDependencies; n++) {
			tmp[jobs[i].dependencies[n]] += 1;
		}
	}

	return tmp;
}

JobWrapper* Scheduler::wrapJobs(Job *jobs) {
	JobWrapper* tmp = new JobWrapper[num_jobs];

	for(int i = 0; i < num_jobs; i++) {
		JobWrapper jw;

		jw.uid = i;
		jw.job = jobs[i];

		tmp[i] = jw;
	}

	return tmp;
}

JobWrapper* Scheduler::topSort() {
	int dep_cpy[num_jobs], tmp_pos = 0;
	JobWrapper* tmp = new JobWrapper[num_jobs];
	queue<JobWrapper> q, wave;

	memcpy(dep_cpy, dep_chart, num_jobs * sizeof(int));

	// Find 0 in-degree jobs
	for(int i = 0; i < num_jobs; i++) {
		if(dep_cpy[jl[i].uid] == 0) 
			q.push(jl[i]);
	}

	while(!q.empty()) {
		JobWrapper cur = q.front();
		
		for(int i = 0; i < cur.job.numDependencies; i++) {
			int cur_uid = cur.job.dependencies[i];
			
			if(--dep_cpy[cur_uid] == 0) {
				q.push(jl[cur_uid]);
			}
		}

		tmp[tmp_pos++] = cur;
		q.pop();
	}

	return tmp;
}


/*
void Scheduler::topSort(Job *jobs, TopSortCB callback = NULL) {

		//cout << (this->*callback)(&jw, 0, 0 ) << endl;	
}
*/

void Scheduler::calcWaveST(JobWrapper* wave, int wave_size, int prev_ect) {
	for(int i = 0; i < wave_size; i++) 
		wave[i].st = prev_ect - wave[i].job.startTime;
}

int Scheduler::calcWaveECT(JobWrapper* wave, int wave_size, int prev_ct) {
	int ect = 0;

	for(int i = 0; i < wave_size; i++) {
		wave[i].job.startTime = prev_ct + wave[i].job.length;
		
		if(wave[i].job.startTime > ect)
			ect = wave[i].job.startTime;
	}

	return ect;
}


/* --------------------
 *   Run scheduler
 * --------------------
 */

void Scheduler::run()
{
	// Create a copy of the dependency chart
	int dep_cpy[num_jobs], marker[1000];
	int marker_pos = 0;
	JobWrapper jwt;
	memcpy(dep_cpy, dep_chart, num_jobs * sizeof(int));

	// Begin assigning people
	while(!crit.empty() && !reg.empty()) {

		// Start with the critical jobs
		jwt = crit.front();
		while(dep_cpy[jwt.uid] == 0) {
			assignPeople(jwt);
			
			// Mark for dependency deletion
			for(int i = 0; i < jwt.job.numDependencies; ++i) {
				marker[marker_pos] = jwt.job.dependencies[i];
				marker_pos += 1;
			}

			crit.pop();
			jwt = crit.front();
		} 	
		
		// Look at the regular jobs
		jwt = reg.front();
		while(dep_cpy[jwt.uid] == 0) {
			assignPeople(jwt);
			
			// Mark for dependency deletion 
			for(int i = 0; i < jwt.job.numDependencies; ++i) {
				marker[marker_pos] = jwt.job.dependencies[i];
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


void Scheduler::queueJobs() {
	for(int i = 0; i < num_jobs; i++) {
		if(jl[i].st == 0) crit.push(jl[i]);	
		else reg.push(jl[i]);
	}
}


void Scheduler::assignPeople(JobWrapper &jw) {
	cout << "Assigning people" << endl;
}


/* -------------------
 *   Debug methods
 * -------------------
 */

void Scheduler::printDepChart() {
	cout << " ===== PRINTING Dep Chart ===== " << endl;
	
	for(int i = 0; i < num_jobs; i++) {
		cout << i << " => " << dep_chart[i] << endl;
	}
}

void Scheduler::printJl() {
	cout << " ===== PRINTING JL =====" << endl;

	for(int i = 0; i < num_jobs; i++) {
		JobWrapper tmp = jl[i];

		cout << "Job UID: " << tmp.uid << endl;
		cout << "\tLength: " << tmp.job.length << endl;
		cout << "\tStart time: " << tmp.job.startTime << endl;
		cout << "\tDependencies: ";
		
		for(int n = 0; n < tmp.job.numDependencies; n++) {
			cout << tmp.job.dependencies[n] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Scheduler::testRun() {
	for(int i = 0; i < num_jobs / 2; i++) {
		reg.push(jl[i]);
	}

	for(int i = num_jobs / 2; i < num_jobs; i++) {
		crit.push(jl[i]);
	}
}
