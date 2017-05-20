#include "Scheduler.h"
#include "DES_Layer.h"
#include "FCFS.h"
#include "LCFS.h"
#include "SJF.h"
#include "RR.h"
#include "PRIO.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>
#include <unistd.h>
#include <limits.h>
#include <algorithm>

using namespace std;

int randArray[1000000];
int countRand;
int ofs = 0;
int quantum = INT_MAX;
int algo = 0;
double cpu_util = 0.0000;
double io_util  = 0.0000;

// read random numbers
void readRandom(char* rfilename) {
	ifstream rFile(rfilename, ios::in);
	string line;
	getline(rFile, line);
	countRand = atoi(line.c_str());
	for (int i = 0; i < countRand; i++) {
		getline(rFile, line);
		randArray[i] = atoi(line.c_str());
	}
	rFile.close();
}

// create random number
int myRandom(int num) {
	int temp = 0;
	if (ofs < countRand) {
		temp = ofs;
		ofs++;
		return 1 + randArray[temp] % num;
	}
	else {
		temp = ofs - countRand;
		ofs++;
		return 1 + randArray[temp] % num;
	}
}

// read input file
void readInput(char* filename, DesLayer* dtemp) {
	int pid = 0;
	ifstream input(filename, ios::in);
	string line;
	while (getline(input, line)) {
		int SPrio = myRandom(4); // create a static priority
		istringstream iss(line);
		int a[4]; // create an array to store: AT, TC, CB, IB
		if (!(iss >> a[0] >> a[1] >> a[2] >> a[3])) {
			break;
		}
		else {
			// create process and event
			int AT = a[0];
			int TC = a[1];
			int CB = a[2];
			int IO = a[3];
			Process* ptemp = new Process(pid, AT, TC, CB, IO, SPrio);
			dtemp->insertProcess(ptemp);
			// When encounter a new process, initialize a start event, transition is from Created to Ready.
			// Event: Created -> Ready, its time stamp is arriving time.
			Event* evt = dtemp->initEvent(ptemp, AT);
			dtemp->insertEvent(evt);
			pid++;
		}
	}
	input.close();
}

// create cpu burst
int cpuBurst(int cb) {
	return myRandom(cb);
}

// create io burst
int ioBurst(int io) {
	return myRandom(io);
}

bool parseInput(int argc, char** argv, bool& v, string& s) {
	int c;
	opterr = 0;
	while ((c = getopt(argc, argv, "vs:")) != -1) {
		switch (c) {
		case 'v': {
			v = true;
			break;
		}
		case 's': {
			s = optarg;
			break;
		}
		default: {
			abort();
		}
		}
	}
	return true;
}

Scheduler* chooseAlgo(string s) {
	if(s.compare("F") == 0) {
		algo = 1;
		return new FCFS();
	}
	else if(s.compare("L") == 0) {
		algo = 2;
		return new LCFS();
	}
	else if(s.compare("S") == 0) {
		algo = 3;
		return new SJF();
	}
	else if(s.substr(0, 1).compare("R") == 0) {
		algo = 4;
		quantum = atoi(s.substr(1).c_str());
		return new RR();
	}
	else if(s.substr(0, 1).compare("P") == 0) {
		algo = 5;
		quantum = atoi(s.substr(1).c_str());
		return new PRIO();
	}
	else {
		cout << "Wrong User Input Option!" << endl;
		exit(0);
	}
}

// transition -1: trans_to_created
// transition 0: trans_to_ready
// transition 1: trans_to_run
// transition 2: trans_to_block
// transition 3: trans_to_preempt

void simulation(DesLayer* dLayer, Scheduler* sch, bool verbose, int quantum) {

	int curTime = 0;
	int ioRun = 0;
	bool ioStatus = false;
	int ioStart = 0;
	bool callScheduler = false;
	Event* evt;
	Event* CURRENT_RUNNING_PROCESS = NULL;

	while ((evt = dLayer->getEvent()) != NULL) {

		curTime = evt->timeStamp;
		evt->process->resolveTime(evt->process, curTime);

		int trans = evt->transition;
		switch (trans) {
		case -1: {
			// created
			Event* newEvt = dLayer->createEvent(evt, "ready");
			dLayer->insertEvent(newEvt);
			dLayer->resolveVerbose(evt, verbose, curTime, -1, 0);
			callScheduler = true;
			break;
		}	
		case 0: {
			// trans_to_ready
			if(evt->process->DPrio == -1) {
				evt->process->DPrio = evt->process->SPrio - 1;
				sch->addExpProcess(evt);
			}
			else {
				sch->addActProcess(evt);
			}
			callScheduler = true;
			break;
		}
		case 1: {
			// trans_to_run
			CURRENT_RUNNING_PROCESS = NULL;
			callScheduler = true;
			evt->process->cpuRem -= evt->process->preStateTime;
			if(evt->process->cpuRem == 0) {
				// done
				evt->process->FT = curTime;
				evt->process->TT = curTime - evt->process->AT;
				dLayer->resolveVerbose(evt, verbose, curTime, 0, 0);
				break;
			}
			else {
				// check if this process has ran before
				if(evt->cpuStatus) {
					evt->cbRem -= evt->process->preStateTime;
					if(evt->cbRem > 0) {
						// create event for preemption
						Event* newEvt = dLayer->createEvent(evt, "preemption");
						newEvt->cbRem = evt->cbRem;
						newEvt->cpuStatus = true;
						newEvt->process->DPrio--;
						dLayer->insertEvent(newEvt);
						dLayer->resolveVerbose(evt, verbose, curTime, 2, 0);
						break;
					}
					else {
						// create event for block
						int ioburst = ioBurst(evt->process->IO);
						// when first get an io process, start record io time
						if(!ioStatus) {
							ioStart = curTime;
							ioStatus = true;
						}
						ioRun++;
						evt->timeStamp += ioburst;
						evt->process->ioWait(evt->process, ioburst);
						Event* newEvt = dLayer->createEvent(evt, "block");
						newEvt->cpuStatus = false; // finish one cpu burst
						dLayer->insertEvent(newEvt);	
						dLayer->resolveVerbose(evt, verbose, curTime, 1, ioburst);
						break;
					}
				}
				else {
					int ioburst = ioBurst(evt->process->IO);
					// when first get an io process, record io start time
					if(!ioStatus) {
						ioStart = curTime;
						ioStatus = true;
					}
					ioRun++;
					evt->timeStamp += ioburst;
					evt->process->ioWait(evt->process, ioburst);
					Event* newEvt = dLayer->createEvent(evt, "block");
					dLayer->insertEvent(newEvt);
					dLayer->resolveVerbose(evt, verbose, curTime, 1, 0);
					break;
				}
			}
		}
		case 2: {
			// trans_to_block
			callScheduler = true;
			ioRun--;
			// no io status, record io time
			if(ioRun == 0) {
				ioStatus = false;
				io_util += curTime - ioStart;
			}
			// create next event
			Event* newEvt = dLayer->createEvent(evt, "ready");
			// set priority back to Static Priority - 1
			newEvt->process->DPrio = newEvt->process->SPrio - 1;
			dLayer->insertEvent(newEvt);
			dLayer->resolveVerbose(evt, verbose, curTime, 3, 0);
			break;
		}
		case 3: {
			// trans_to_preemption
			callScheduler = true;
			if(evt->process->DPrio == -1) {
				evt->process->DPrio = evt->process->SPrio - 1;
				sch->addExpProcess(evt);
			}
			else {
				sch->addActProcess(evt);
			}
			break;
		}
		}
		// delete the first event
		dLayer->deleteEvent();

		if(callScheduler) {
			// get events executed at the same time
			if(dLayer->getNextEvtTime() == curTime) {
				continue; 
			}
			callScheduler = false;
			if(CURRENT_RUNNING_PROCESS == NULL) {
				CURRENT_RUNNING_PROCESS = sch->getNextProcess();
				if(CURRENT_RUNNING_PROCESS == NULL) {
					continue;
				}

				CURRENT_RUNNING_PROCESS->process->resolveTime(CURRENT_RUNNING_PROCESS->process, curTime);
				// record time process in ready state (cpu wait time)
				CURRENT_RUNNING_PROCESS->process->cpuWait(CURRENT_RUNNING_PROCESS->process);

				if(!CURRENT_RUNNING_PROCESS->cpuStatus) {
				// first run
					CURRENT_RUNNING_PROCESS->cpuStatus = true;
					int cpuburst = cpuBurst(CURRENT_RUNNING_PROCESS->process->CB);
					CURRENT_RUNNING_PROCESS->cbRem = cpuburst;
					int rem = CURRENT_RUNNING_PROCESS->process->cpuRem;
					int minTimeStep = min(cpuburst, min(quantum, rem));
					int realBurst = min(cpuburst, rem);
					// create new state for running
					CURRENT_RUNNING_PROCESS->timeStamp = curTime + minTimeStep; 
					Event* newEvt = dLayer->createEvent(CURRENT_RUNNING_PROCESS, "running");
					newEvt->cbRem = realBurst;
					newEvt->cpuStatus = true;
					dLayer->insertEvent(newEvt);
				}
				else {
				// already experienced a preemption state, cpuburst <= cpuRem
					int minTimeStep = min(quantum, CURRENT_RUNNING_PROCESS->cbRem); 
					CURRENT_RUNNING_PROCESS->timeStamp = curTime + minTimeStep;
					Event* newEvt = dLayer->createEvent(CURRENT_RUNNING_PROCESS, "running");
					newEvt->cbRem = CURRENT_RUNNING_PROCESS->cbRem;
					newEvt->cpuStatus = true;
					dLayer->insertEvent(newEvt);
				}
				dLayer->resolveVerbose(evt, verbose, curTime, 4, 0);
			}
		}
	}
	delete CURRENT_RUNNING_PROCESS;
	delete evt;
}

void printSummary(DesLayer* des, int algoNumber) {
	switch(algo) {
		case 0: {
			cout << "No input algorithm!" << endl;
			break;
		}
		case 1: {
			cout << "FCFS" << endl;
			break;
		}
		case 2: {
			cout << "LCFS" << endl;
			break;
		}
		case 3: {
			cout << "SJF" << endl;
			break;
		}
		case 4: {
			cout << "RR " << quantum << endl;
			break;
		}
		case 5: {
			cout << "PRIO " << quantum << endl;
			break;
		}
	}

	int    maxfintime = 10;
	double avg_turnaround = 0.0;
	double avg_waittime = 0.0;
	double throughput = 0.0000;
	double cpu_util = 0.0000;

	for(list<Process*>::iterator it = des->processList.begin(); it != des->processList.end(); it++) {
		Process* p = *it;
		// get the final time, which is the largest finishing time for all processes
		maxfintime = max(maxfintime, p->preTime);
		cpu_util += p->TC;
		avg_waittime += p->CW;
		avg_turnaround += p->TT;

		printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
	       p->pid,
	       p->AT, p->TC, p->CB, p->IO, p->SPrio,
	       p->FT, // finishing time
	       p->TT, // turnaround time
	       p->IT, // time in blocked state
	       p->CW); // time in ready state
	}

	// compute the following variables based on the simulation
	// and the final state of all the processes 

	int num = des->processList.size();
	avg_turnaround = avg_turnaround / num;	
	avg_waittime = avg_waittime / num;
	throughput = double(num * 100) / maxfintime;

	printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
	       maxfintime,
	       cpu_util * 100.00 / double(maxfintime),
	       io_util * 100.00 / double(maxfintime),
	       avg_turnaround,
	       avg_waittime, 
	       throughput);
}

int main(int argc, char** argv) {
	DesLayer* des = new DesLayer();
	Scheduler* sch;
	bool verbose = false; // default verbose
	string algospec = "F"; // default algorithm

	// user input options
	if(!parseInput(argc, argv, verbose, algospec)) {
		exit(0);
	}

	sch = chooseAlgo(algospec);
	readRandom(argv[argc - 1]);
	readInput(argv[argc - 2], des);
	simulation(des, sch, verbose, quantum);
	printSummary(des, algo);

	return 1;
}



