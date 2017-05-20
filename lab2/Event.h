#ifndef EVENT_H
#define EVENT_H

#include "Process.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

class Event {
public:
	int timeStamp;
	Process* process;
	int transition;
	int cbRem;
	bool cpuStatus;
	Event(Process* p, int ts, int trans);
	~Event(){delete process;};
	void printOldState(Event* e, int time, int type, int trans);
};

#endif