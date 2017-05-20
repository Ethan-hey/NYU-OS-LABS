#include "LCFS.h"
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

bool LCFS::isEmpty() {
	return (actqueue.size() == 0);
}

Event* LCFS::getNextProcess() {
	if(isEmpty()) {
		return NULL;
	}
	Event* temp = actqueue.back();
	actqueue.pop_back();
	return temp;
}

void LCFS::addActProcess(Event* evt) {
	actqueue.push_back(evt);
}

void LCFS::addExpProcess(Event* evt) {
	actqueue.push_back(evt);
}
