#include "Event.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

// transition -1: trans_to_created
// transition 0: trans_to_ready
// transition 1: trans_to_run
// transition 2: trans_to_block
// transition 3: trans_to_preempt

Event::Event(Process* pptr, int ts, int trans) {
	timeStamp = ts;
	process = pptr;
	transition = trans;
	cpuStatus = false;
}

void Event::printOldState(Event* evt, int curTime, int type, int ioburst) {
	switch(type) {
		case -1: {
			cout << curTime << " "
			     << evt->process->pid << " "
			     << evt->process->preStateTime
			     << ": "
			     << "CREATED -> READY" << endl;
			break;
		}

		case 0: {
			cout << curTime << " "
			     << evt->process->pid << " "
			     << evt->process->preStateTime
			     << ": "
			     << "Done" << endl;
			break;
		}

		case 1: {
			cout << curTime << " "
			     << evt->process->pid << " "
			     << evt->process->preStateTime
			     << ": "
			     << "RUNNG -> BLOCK  "
			     << "ib=" << ioburst
			     << " rem=" << evt->process->cpuRem << endl;
			break;
		}
		case 2: {
			cout << curTime << " "
			     << evt->process->pid << " "
			     << evt->process->preStateTime
			     << ": "
			     << "RUNNG -> READY  "
			     << "cb=" << ioburst
			     << " rem=" << evt->process->cpuRem
			     << " prio=" << evt->process->DPrio
			     << endl;
			break;
		}
		case 3: {
			cout << curTime << " "
			     << evt->process->pid << " "
			     << evt->process->preStateTime
			     << ": "
			     << "BLOCK -> READY" << endl;
			break;
		}
		case 4: {
			cout << curTime << " "
			     << evt->process->pid << " "
			     << evt->process->preStateTime
			     << ": "
			     << "READY -> RUNNG "
			     << "cb=" << ioburst
			     << " rem=" << evt->process->cpuRem
			     << " prio=" << evt->process->DPrio
			     << endl;
			break;
		}
	}
}


