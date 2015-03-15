#include <iostream>
#include <cstring>

#include "scheduler.h"
#include "JobWrapper.h"

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
	
	// For debugger purposes
	printCriChart();
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
		jw.job.startTime = -1;
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
	queue<JobWrapper> q, wave, empty;
	JobWrapper* tmp = new JobWrapper[num_jobs];
	
	memcpy(dep_cpy, dep_chart, num_jobs * sizeof(int));

	// Find 0 in-degree jobs
	for(int i = 0; i < num_jobs; i++) {
		if(dep_cpy[jl[i].uid] == 0) 
			wave.push(jl[i]);
	}

	while(!wave.empty()) {
		JobWrapper cur = wave.front();
		cur.wave = wave_num;
		
		for(int i = 0; i < cur.job.numDependencies; i++) {
			int cur_uid = cur.job.dependencies[i];
			
			if(--dep_cpy[cur_uid] == 0) {
				q.push(jl[cur_uid]);
			}
		}

		tmp[tmp_pos] = cur;
		key_chart[cur.uid] = tmp_pos++;
		
		// Prepare for next wave
		wave.pop();
		if(wave.empty()) {
			wave = q;
			q = empty;
			wave_num += 1;
		}
	}

	return tmp;
}

void Scheduler::calcTimes(JobWrapper *tmp) {
	queue<JobWrapper> wave, empty;
	int cur_wave = tmp[0].wave;
	JobWrapper jw;
	
	// Initialize the wrapper
	jw.uid = -1;
	jw.job.finishTime = 0;

	for(int i = 0; i < num_jobs; i++) {
	
		// Seperate the jobs into waves
		if(tmp[i].wave == cur_wave) {
			wave.push(tmp[i]);
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
			wave.push(tmp[i]);
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
	return;
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

void Scheduler::calcWaveST(queue<JobWrapper> wave, int prev_ect, JobWrapper *tmp) {
	while(!wave.empty()) {
		int cur_uid = key_chart[wave.front().uid];
		
		// Slack time = pevious ECT - finish time
		tmp[cur_uid].st = prev_ect - tmp[cur_uid].job.finishTime;
		wave.pop();
	}
}

JobWrapper Scheduler::calcWaveECT(queue<JobWrapper> wave, JobWrapper jw, JobWrapper *tmp) {
	JobWrapper ect;
	int replaceable = true;
	int prev_ect = jw.job.finishTime;
	
	while(!wave.empty()) {
		int cur_uid = key_chart[wave.front().uid];
		int finish_time = prev_ect + tmp[cur_uid].job.length;
		int num_dep = tmp[cur_uid].job.numDependencies;
		
		// Set start times to previous ect
		tmp[cur_uid].job.startTime = prev_ect;
		tmp[cur_uid].job.finishTime = finish_time;
	
		// Find the greatest completion time
		if(finish_time > ect.job.finishTime || replaceable) {
			
			if(checkIsDependent(jw, wave.front().uid)) {
				if(num_dep > 0) {
					ect = tmp[cur_uid];
					replaceable = false;
				}
				
				if(num_dep == 0 && replaceable) 
					ect = tmp[cur_uid];
			}

		}
			
		wave.pop();
	}
	
	return ect;
}

void Scheduler::queueJobs() {
	int i = 0;
	queue<int> crit_id;

	for(; i < cri_leng; ++i) {
		if(cri_chart[i] == -1) continue;
		
		crit_id.push(cri_chart[i]);
		crit.push(jl[key_chart[cri_chart[i]]]);
	}

	for(i = 0; i < num_jobs; ++i) {
		if(!crit_id.empty()) {
			if(crit_id.front() == jl[i].uid) {
				crit_id.pop();
				continue;
			}
		}

		reg.push(jl[i]);
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

void Scheduler::printQueue(queue<JobWrapper> q) {
	int i = 0;
	queue<JobWrapper> tmp_q = q;
	JobWrapper *tmp_ar = new JobWrapper[num_jobs];
	
	while(!tmp_q.empty()) {
		tmp_ar[i++] = tmp_q.front();		
		tmp_q.pop();
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


