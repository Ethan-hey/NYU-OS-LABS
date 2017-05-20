#ifndef PRIO_H
#define PRIO_H

#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

class PRIO : public Scheduler {
public:
	PRIO(){};
	~PRIO(){};	
	void addActProcess(Event*);
	void addExpProcess(Event*);
	bool isEmpty();
    Event* getNextProcess();
	list<Event*> actqueue;
	list<Event*> expqueue;

private:

};

#endif