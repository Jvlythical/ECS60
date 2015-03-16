#include <iostream>
#include <cstring>

#include "scheduler.h"
#include "JobWrapper.h"
#include "QueueLL.h"

using namespace std;

Scheduler::Scheduler(int numJobs, int numWorkers, Job *jobs, int numPeople)
{
	key_chart = new int[numJobs];
	cri_chart = new int[numJobs];

	num_jobs = numJobs;
	dep_chart = createDepChart(jobs);
	jl = wrapJobs(jobs);
	jl = topSort();
	calcTimes(jl);
	queueJobs();
	
	//printJl(jl, num_jobs);
	// For debugging purposes
	//printCriChart();
	printQueue(crit);
	//printJl(jl, num_jobs);
} // Scheduler()

Scheduler::~Scheduler() {
	delete(dep_chart);
	delete(key_chart);
	delete(cri_chart);
	delete(jl);
}

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
		jw.job.startTime = 0;
		jw.job.finishTime = -1;

		tmp[i] = jw;
	}

	return tmp;
}

/* ---------------------
 *   Set-up Methods 
 * ---------------------
 */

JobWrapper* Scheduler::topSort() {
	int dep_cpy[num_jobs], tmp_pos = 0, wave_num = 0;
	QueueLL<JobWrapper> q, wave, empty;
	JobWrapper* tmp = new JobWrapper[num_jobs];
	
	memcpy(dep_cpy, dep_chart, num_jobs * sizeof(int));

	// Find 0 in-degree jobs
	for(int i = 0; i < num_jobs; i++) {
		if(dep_cpy[jl[i].uid] == 0) 
			wave.enqueue(jl[i]);
	}

	while(!wave.isEmpty()) {
		JobWrapper cur = wave.getFront();
		cur.wave = wave_num;
		
		for(int i = 0; i < cur.job.numDependencies; i++) {
			int cur_uid = cur.job.dependencies[i];
			
			if(--dep_cpy[cur_uid] == 0) {
				q.enqueue(jl[cur_uid]);
			}
		}

		tmp[tmp_pos] = cur;
		key_chart[cur.uid] = tmp_pos++;
		
		// Prepare for next wave
		wave.dequeue();
		if(wave.isEmpty()) {
			wave = q;
			q = empty;	
			wave_num += 1;
		}
	}

	return tmp;
}

void Scheduler::calcTimes(JobWrapper *tmp) {
	QueueLL<JobWrapper> wave, empty;
	int cur_wave = tmp[0].wave;
	JobWrapper jw;
	
	// Initialize the wrapper
	jw.uid = -1;
	jw.job.startTime = 0;
	jw.job.finishTime = 0;

	for(int i = 0; i < num_jobs; i++) {
	
		// Seperate the jobs into waves
		if(tmp[i].wave == cur_wave) {
			wave.enqueue(tmp[i]);
		}
		else {
			jw = calcWaveECT(wave, jw, tmp);
			calcWaveST(wave, jw.job.finishTime, tmp);
			
			// Set-up critical path
			if(jw.uid != -1) {
				if(cri_chart[cri_leng - 1] == jw.uid) 
					cri_chart[cri_leng] = -1;
				else 
					cri_chart[cri_leng] = jw.uid; 
				cri_leng += 1;
			}
			
			// Initialize next wave
			wave = empty;
			wave.enqueue(tmp[i]);
			cur_wave = tmp[i].wave;
		}
	}

	jw = calcWaveECT(wave, jw, tmp);
	calcWaveST(wave, jw.job.finishTime, tmp);

	if(jw.uid != -1) {
		if(cri_chart[cri_leng - 1] == jw.uid) 
			cri_chart[cri_leng] = -1;
		else 
			cri_chart[cri_leng] = jw.uid; 

		cri_leng += 1;
	}

}

/* --------------------
 *   Run scheduler
 * --------------------
 */

void Scheduler::run()
{
	/*
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
	*/
} // run()

void Scheduler::assignPeople(JobWrapper &jw) {
	cout << "Assigning people" << endl;
}

/* ---------------------
 *   Helper methods
 * ---------------------
 */

bool Scheduler::checkIsDependent(JobWrapper parent, int uid) {
	bool valid = false;
	
	if(parent.uid == -1) return true;
	for(int i = 0; i < parent.job.numDependencies; i++) {
		if(parent.job.dependencies[i] == uid) {
			valid = true;
			break;
		}
	}

	return valid;
}

void Scheduler::calcWaveST(QueueLL<JobWrapper> wave, int prev_ect, JobWrapper *tmp) {
	while(!wave.isEmpty()) {
		int cur_uid = key_chart[wave.getFront().uid];
		
		// Slack time = pevious ECT - finish time
		tmp[cur_uid].st = prev_ect - tmp[cur_uid].job.finishTime;
		wave.dequeue();
	}
}

JobWrapper Scheduler::calcWaveECT(QueueLL<JobWrapper> wave, JobWrapper jw, JobWrapper *tmp) {
	JobWrapper ect;
	bool replaceable = true;
	int finish_time, tmp_uid;
	int prev_ect = jw.job.finishTime;
	
	while(!wave.isEmpty()) {
		int cur_uid = key_chart[wave.getFront().uid];
		int num_dep = tmp[cur_uid].job.numDependencies;

		// Set start times to previous ect
		if(checkIsDependent(jw, wave.getFront().uid))
			tmp[cur_uid].job.startTime = prev_ect;
		
		// Set the finish time
		finish_time = tmp[cur_uid].job.startTime + tmp[cur_uid].job.length;
		tmp[cur_uid].job.finishTime = finish_time;

		// Set the start times for the job's dependencies
		for(int i = 0; i < tmp[cur_uid].job.numDependencies; i++) {
			tmp_uid = key_chart[tmp[cur_uid].job.dependencies[i]];
			
			if(finish_time > tmp[cur_uid].job.startTime)
				tmp[tmp_uid].job.startTime = finish_time;
		}
	
		// Find the greatest completion time
		if(finish_time > ect.job.finishTime || replaceable) {
			
			if(checkIsDependent(jw, wave.getFront().uid)) {
				if(num_dep > 0) {
					ect = tmp[cur_uid];
					replaceable = false;
				}
				
				if(num_dep == 0 && replaceable) 
					ect = tmp[cur_uid];
			}

		}
			
		wave.dequeue();
	}
	
	return ect;
}

void Scheduler::queueJobs() {
	int i = 0;
	QueueLL<int> crit_id;

	for(; i < cri_leng; ++i) {
		if(cri_chart[i] == -1) continue;
		
		crit_id.enqueue(cri_chart[i]);
		crit.enqueue(jl[key_chart[cri_chart[i]]]);
	}

	for(i = 0; i < num_jobs; ++i) {
		if(!crit_id.isEmpty()) {
			if(crit_id.getFront() == jl[i].uid) {
				crit_id.dequeue();
				continue;
			}
		}

		reg.enqueue(jl[i]);
	}
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

void Scheduler::printKeyChart() {
	cout << " ===== PRINTING Key Chart ===== " << endl;
	
	for(int i = 0; i < num_jobs; i++) {
		cout << i << " => " << key_chart[i] << endl;
	}

}

void Scheduler::printJl(JobWrapper *jw_ar, int size = 0) {
	cout << " ===== PRINTING JL =====" << endl;

	for(int i = 0; i < size; i++) {
		JobWrapper tmp = jw_ar[i];

		cout << "Job UID: " << tmp.uid << endl;
		cout << "\tLength: " << tmp.job.length << endl;
		cout << "\tStart time: " << tmp.job.startTime << endl;
		cout << "\tFinish time: " << tmp.job.finishTime << endl;
		cout << "\tSlack time: " << tmp.st << endl;
		cout << "\tWave: " << tmp.wave << endl;
		cout << "\tDependencies: ";
		
		for(int n = 0; n < tmp.job.numDependencies; n++) {
			cout << tmp.job.dependencies[n] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Scheduler::printQueue(QueueLL<JobWrapper> q) {
	int i = 0;
	QueueLL<JobWrapper> tmp_q = q;
	JobWrapper *tmp_ar = new JobWrapper[num_jobs];
	
	while(!tmp_q.isEmpty()) {
		tmp_ar[i++] = tmp_q.getFront();		
		tmp_q.dequeue();
	}

	printJl(tmp_ar, i);

	delete(tmp_ar);
}

void Scheduler::printCriChart() {
	cout << " ===== PRINTING Critical Chart ===== " << endl;
	
	for(int i = 0; i < cri_leng; i++) {
		cout << i << " \t=>\t" << cri_chart[i] << endl;
	}
}


