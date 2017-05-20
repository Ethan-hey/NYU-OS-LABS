#ifndef LCFS_H
#define LCFS_H

#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

class LCFS : public Scheduler {
public:
	LCFS(){};
	~LCFS(){};
	void addActProcess(Event*);
	void addExpProcess(Event*);
	bool isEmpty();
    Event* getNextProcess();
	list<Event*> actqueue;

private:

};

#endif