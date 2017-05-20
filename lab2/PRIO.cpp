#include "PRIO.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>
#include <unistd.h>
#include <limits.h>

using namespace std;

bool PRIO::isEmpty() {
	return (actqueue.size() == 0) && (expqueue.size() == 0); 
}

Event* PRIO::getNextProcess() {
    if(isEmpty()) {
    	return NULL;
    }
    if(actqueue.size() == 0 && expqueue.size() != 0) {
    	while(expqueue.size() != 0) {
    		actqueue.push_back(expqueue.front());
    		expqueue.pop_front();    		
    	}
    }

    Event* evt = actqueue.front();
    actqueue.pop_front();
    return evt;
}

void PRIO::addExpProcess(Event* evt) {
	for (list<Event*> :: iterator it = expqueue.begin(); it != expqueue.end();) {
		if((*it)->process->DPrio < evt->process->DPrio) {
			expqueue.insert(it, evt);
			return;
		}
		else it++;
	}
	expqueue.push_back(evt);
}

void PRIO::addActProcess(Event* evt) {
	for (list<Event*> :: iterator it = actqueue.begin(); it != actqueue.end();) {
		if((*it)->process->DPrio < evt->process->DPrio) {
			actqueue.insert(it, evt);
			return;
		}
		else it++;
	}
	actqueue.push_back(evt);
}

