Rf#include "DES_Layer.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <list>

using namespace std;

DesLayer::DesLayer() {
	list<Event*> eventList;
	list<Process*> processList;
}

// transition -1: initialization
// transition 0: trans to ready
// transition 1: trans to run
// transition 2: trans to block
// transition 3: trans to preemption

Event* DesLayer::initEvent(Process* p, int ts) {
	// start a new process, event: created -> ready, transition: ready
	Event* startEvent = new Event(p, ts, -1);
	return startEvent;
}

Event* DesLayer::createEvent(Event* evt, string s) {
	// get current event and create the next event
	// create four types of events: block, preemption, ready, running
	if (s == "block") {
		Event* newEvent = new Event(evt->process, evt->timeStamp, 2);
		return newEvent;
	}
	else if (s == "preemption") {
		Event* newEvent = new Event(evt->process, evt->timeStamp, 3);
		return newEvent;
	}
	else if (s == "ready") {
		Event* newEvent = new Event(evt->process, evt->timeStamp, 0);
		return newEvent;
	}
	else {
		Event* newEvent = new Event(evt->process, evt->timeStamp, 1);
		return newEvent;
	}
}

// insert an event according to timpStamp order
void DesLayer::insertEvent(Event* e) {
	for (list<Event*>::iterator it = eventList.begin(); it != eventList.end();) {
		if((*it)->timeStamp > e->timeStamp) {
			eventList.insert(it, e);
			return;
		}
		else it++;
	}
	eventList.push_back(e);
}

Event* DesLayer::getEvent() {
	return eventList.front();
}

int DesLayer::getNextEvtTime() {
	if(eventList.size() == 0) {
		return -1;
	}
	return eventList.front()->timeStamp;
}

void DesLayer::deleteEvent() {
	eventList.pop_front();
}

void DesLayer::insertProcess(Process* p) {
	processList.push_back(p);
}

void DesLayer::resolveVerbose(Event* evt, bool v, int time, int printType, int ioburst) {
	if(v) {
		switch(printType) {
			case -1: evt->printOldState(evt, time, printType, 0);
			case 0: evt->printOldState(evt, time, printType, 0);
			case 1: evt->printOldState(evt, time, printType, ioburst);
			case 2: evt->printOldState(evt, time, printType, 0);
			case 3: evt->printOldState(evt, time, printType, 0);
			case 4: evt->printOldState(evt, time, printType, 0);
		}
	}
}
