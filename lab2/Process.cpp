#include "Process.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <list>
#include <unistd.h>
#include <limits.h>

using namespace std;

Process::Process(int p, int at, int tc, int cb, int io, int sp) {
	pid = p;
	AT = at;			
	TC = tc;			
	CB = cb;			
	IO = io;
	FT = 0;	
	TT = 0;	
	IT = 0;
	CW = 0;	
	SPrio = sp;      
	DPrio = sp - 1;
	preTime = at;
	preStateTime = 0;  
	cpuRem = tc;
}

void Process::resolveTime(Process* p, int cur) {
	// calculate time staying in previous state
	p->preStateTime = cur - p->preTime;
	// set current time as next event's previous time
	p->preTime = cur;
}

void Process::cpuWait(Process* p) {
	p->CW += p->preStateTime;
}

void Process::ioWait(Process* p, int io) {
	p->IT += io;
}