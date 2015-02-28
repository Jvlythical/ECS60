family.out : familyRunner.o familytree.o  
	g++ -ansi -pg -Wall -g -o  family.out familyRunner.o familytree.o  

familyRunner.o : familyRunner.cpp familyRunner.h CPUTimer.h familytree.h
	g++ -ansi -pg -Wall -g -c familyRunner.cpp

familytree.o : familytree.cpp familytree.h hw3/HashTable.h PersonWrapper.h
	g++ -ansi -pg -Wall -g -c familytree.cpp

clean : 
	rm -f family.out familyRunner.o  familytree.o   
