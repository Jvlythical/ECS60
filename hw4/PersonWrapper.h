#ifndef PERSONWRAPPER_H
#define	PERSONWRAPPER_H

#include "familyRunner.h"

struct PersonWrapper {
	int uid;
	int p1_uid;
	int p2_uid;
	Person parent[2];
	Person person;
	//Person children[7];
};

#endif	/* PERSONWRAPPER_H */

