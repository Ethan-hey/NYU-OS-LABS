#ifndef FCFS_H
#define FCFS_H

#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

class FCFS : public Scheduler {
public:
	FCFS(){};	
	~FCFS(){};
	void addActProcess(Event*);
	void addExpProcess(Event*);
	bool isEmpty();
    Event* getNextProcess();
	list<Event*> actqueue;

private:

};

#endif