#include "RR.h"
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

bool RR::isEmpty() {
	return (actqueue.size() == 0);
}

Event* RR::getNextProcess() {
	if(isEmpty()) {
		return NULL;
	}
	Event* temp = actqueue.front();
	actqueue.pop_front();
	return temp;
}

void RR::addActProcess(Event* evt) {
	actqueue.push_back(evt);
}

void RR::addExpProcess(Event* evt) {
	actqueue.push_back(evt);
}
