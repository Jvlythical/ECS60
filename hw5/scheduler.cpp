#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>

#include "scheduler.h"
#include "JobWrapper.h"
#include "QueueLL.h"

using namespace std;

Scheduler::Scheduler(int numJobs, int numWorkers, Job *jobs, int numPeople)
{
	key_chart = new int[numJobs];
	cri_chart = new int[numJobs];

	free_ppl = numPeople;
	num_jobs = numJobs;
	num_ppl = numPeople;
	max_ppl = numWorkers;
	
	createPeopleQueue(numPeople);
	dep_chart = createDepChart(jobs);
	jl = wrapJobs(jobs);
	jl = topSort();
	calcTimes(jl);

	queueJobs();

	// Copy jobs over
	for(int i = 0; i < num_jobs; i++) 
		jobs[i] = jl[key_chart[i]].job;

	// For debugging purposes
	//printCriChart();
	//printQueue(crit);
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
		jw.job.finishTime = 0;
		jw.job.numPeopleUsed = 0;

		tmp[i] = jw;
	}

	return tmp;
}

void Scheduler::createPeopleQueue(int num_ppl) {

	for(int i = 0; i < num_ppl; i++) 
		ppl_q.enqueue(i);

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

} // run()

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

int compare(const void *a, const void*b) {
	return  (*(JobWrapper*)b).job.length - (*(JobWrapper*)a).job.length;
}

JobWrapper Scheduler::calcWaveECT(QueueLL<JobWrapper> wave, JobWrapper jw, JobWrapper *tmp) {
	int next_time = 0, count = 0, wave_size = 0;
	int max_time = 0, finish_time, assign_num;
	JobWrapper ect;
	
	JobWrapper jw_ar[500];

	while(!wave.isEmpty()) 
		jw_ar[wave_size++] = wave.dequeue();

	qsort(jw_ar, wave_size, sizeof(JobWrapper), compare);

	for(int i = 0; i < wave_size; i++) 
		wave.enqueue(jw_ar[i]);

	if (wave_size > 100 && num_ppl < 1000) 
		assign_num = 10;
	else
		assign_num = 25;
	if(assign_num > max_ppl)
		assign_num = max_ppl;

	while(!wave.isEmpty()) {
		int cur_uid = key_chart[wave.getFront().uid];

		// Set start times to previous ect
		if(count == 0) {
			if(jw.uid == -1) 
				tmp[cur_uid].job.startTime = 0;
			else
				tmp[cur_uid].job.startTime = jl[key_chart[jw.uid]].job.finishTime;
		}
		else  
			tmp[cur_uid].job.startTime = next_time;
		
		// Set the finish time
		finish_time = tmp[cur_uid].job.startTime + tmp[cur_uid].job.length;
		tmp[cur_uid].job.finishTime = finish_time;

		// Assign the people
		QueueLL<int>parallel_times;
		QueueLL<JobWrapper>parallel_q;
		JobWrapper cur;
		int start_time = tmp[cur_uid].job.startTime;

		while(free_ppl > 0 && !wave.isEmpty()) {
			cur = wave.dequeue();
			cur_uid = key_chart[cur.uid];
			
			tmp[cur_uid].job.startTime = start_time;
			assignPeople(cur, ((free_ppl > assign_num) ? assign_num : free_ppl));
			parallel_times.enqueue(tmp[cur_uid].job.finishTime);
			parallel_q.enqueue(cur);
		}
		

		// Free the people
		while(!parallel_q.isEmpty()) {
			int uid = key_chart[parallel_q.getFront().uid];
			int time = tmp[uid].job.finishTime;
			/*	
			for(int i = 0; i < tmp[uid].job.numDependencies; i++) {
				tmp[key_chart[tmp[uid].job.dependencies[i]]].job.startTime = time;
			}
			*/
		
			// Find the worst case time for the next wave
			if(max_time < time) {
				cur = parallel_q.getFront();
				cur_uid = uid;
				max_time = time;
			}

			freePeople(parallel_q.dequeue());
		}


		// Prepare for next iteration
		if(!wave.isEmpty()) {
			next_time = tmp[cur_uid].job.finishTime;
		} else {
			ect = cur;
		}
		
		++count;
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

bool Scheduler::assignPeople(JobWrapper &tmp, int amnt) {
	short num_ppl = 0;
	JobWrapper &jw = jl[key_chart[tmp.uid]];
	
	// Validation
	if(ppl_q.isEmpty()) return false;

	
	// Assign the people
	for(int i = 0; i < amnt; i++) {
		num_ppl = jw.job.numPeopleUsed;
		
		jw.job.peopleIDs[num_ppl] = ppl_q.dequeue();
		jw.job.numPeopleUsed += 1;

		free_ppl -= 1;
	}

	// Calculate finish time
	jw.job.finishTime = ceil(jw.job.startTime + jw.job.length / (float) jw.job.numPeopleUsed);
	
	return true;
}

void Scheduler::freePeople(const JobWrapper &tmp) {
	JobWrapper &jw = jl[key_chart[tmp.uid]];

	for(int i = 0; i < jw.job.numPeopleUsed; i++) {
		ppl_q.enqueue(jl[key_chart[tmp.uid]].job.peopleIDs[i]);
		free_ppl += 1;
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
		cout << "\tPeople: " << tmp.job.numPeopleUsed << endl;
		cout << "\tPeople: ";
		for(int n = 0; n < tmp.job.numPeopleUsed; n++) {
			cout << tmp.job.peopleIDs[n] << " ";
		}
		cout << endl;
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


