#include "SJF.h"
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

bool SJF::isEmpty() {
    return (actqueue.size() == 0); 
}

Event* SJF::getNextProcess() {
	if(isEmpty()) {
		return NULL;
	}
	Event* temp = actqueue.front();
	actqueue.pop_front();
	return temp;
}

void SJF::addActProcess(Event* evt){             
    for(list<Event*>::iterator it = actqueue.begin(); it != actqueue.end();){
        if((*it)->process->cpuRem > evt->process->cpuRem){
            actqueue.insert(it, evt);
            return;
        }
        else it++;
    }
    actqueue.push_back(evt);
}

void SJF::addExpProcess(Event* evt) {
	for(list<Event*>::iterator it = actqueue.begin(); it != actqueue.end();){
        if((*it)->process->cpuRem > evt->process->cpuRem){
            actqueue.insert(it, evt);
            return;
        }
        else it++;
    }
    actqueue.push_back(evt);
}

