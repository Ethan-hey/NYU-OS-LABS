#include "FCFS.h"
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

bool FCFS::isEmpty() {
	return (actqueue.size() == 0);
}

Event* FCFS::getNextProcess() {
	if(isEmpty()) {
		return NULL;
	}
	Event* temp = actqueue.front();
	actqueue.pop_front();
	return temp;
}

void FCFS::addActProcess(Event* evt) {
	actqueue.push_back(evt);
}

void FCFS::addExpProcess(Event* evt) {
	actqueue.push_back(evt);
}

