#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

class Process {
public:
	int pid;
	int AT;
	int TC;
	int CB;
	int IO;
	int FT;
	int TT;
	int IT;
	int CW;
	int SPrio;
	int DPrio;
	int preTime;
	int preStateTime;
	int cpuRem;
	Process(int p, int at, int tc, int cb, int io, int sp);
	~Process(){};
	void resolveTime(Process* p, int cur);
	void cpuWait(Process* p);
	void ioWait(Process* p, int io);
};

#endif