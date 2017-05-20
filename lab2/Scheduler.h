#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Event.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

using namespace std;

class Scheduler{
public:
	Scheduler();
	~Scheduler(){};
	// storage
	list<Event*> actqueue;
    list<Event*> expqueue;               

    // virtual functions             
    virtual Event* getNextProcess() = 0;
    virtual void addActProcess(Event* e) = 0;
    virtual void addExpProcess(Event* e) = 0;
    virtual bool isEmpty() = 0;
};

#endif