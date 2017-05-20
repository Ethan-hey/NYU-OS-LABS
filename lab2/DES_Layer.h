#ifndef DES_LAYER_H
#define DES_LAYER_H

#include "Event.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

using namespace std;

class DesLayer {
private:

public:
	list<Event*> eventList;
	list<Process*> processList;
	Event* initEvent(Process*, int);
	void insertEvent(Event*);
	Event* createEvent(Event*, string);
	Event* getEvent();
	int getNextEvtTime();
	void deleteEvent();
	void insertProcess(Process*);
	void resolveVerbose(Event* evt, bool, int, int, int);
	DesLayer();
	~DesLayer(){};
};

#endif