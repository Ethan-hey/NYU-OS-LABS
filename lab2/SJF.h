#ifndef SJF_H
#define SJF_H

#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

class SJF : public Scheduler {
public:
	SJF(){};
	~SJF(){};
	void addActProcess(Event*);
	void addExpProcess(Event*);
	bool isEmpty();
    Event* getNextProcess();
	list<Event*> actqueue;

private:

};

#endif