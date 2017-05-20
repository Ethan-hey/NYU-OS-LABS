#ifndef RR_H
#define RR_H

#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

class RR : public Scheduler {
public:
	RR(){};
	~RR(){};
	void addActProcess(Event*);
	void addExpProcess(Event*);
	bool isEmpty();
    Event* getNextProcess();
	list<Event*> actqueue;

private:

};

#endif