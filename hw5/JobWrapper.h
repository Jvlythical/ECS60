#ifndef JOBWRAPPER_H
	#define JOBWRAPPER_H

	typedef struct {
		int uid;
		Job job;
		int st;
		int wave;
		int parent;
	} JobWrapper;

#endif
