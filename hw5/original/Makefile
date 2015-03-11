p5.out: ProjectRunner.o scheduler.o 
	g++ -Wall -ansi -g -o p5.out ProjectRunner.o scheduler.o 


ProjectRunner.o: ProjectRunner.cpp ProjectRunner.h scheduler.h CPUTimer.h 
	g++ -Wall -ansi -g -c ProjectRunner.cpp

scheduler.o: scheduler.cpp scheduler.h 
	g++ -Wall -ansi -g -c scheduler.cpp


clean:
	rm -f p5.out ProjectRunner.o scheduler.o 
