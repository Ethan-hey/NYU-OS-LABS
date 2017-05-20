#include "IO.h"
#include "Request.h"
#include "FIFO.h"
#include "SSTF.h"
#include "SCAN.h"
#include "CSCAN.h"
#include "FSCAN.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>
#include <cmath>
#include <unistd.h>

using namespace std;

string algospec;
bool trace;

// statistics
int tot_turnaround = 0;      // total turnaround time 
int tot_wait = 0;            // total wait time
int total_time = 0;          // total simulated time, i.e. until the last I/O request has completed.
int tot_movement = 0;        // total number of tracks the head had to be moved
double avg_turnaround = 0.0; // average turnaround time per operation from time of submission to time of completion
double avg_waittime = 0.0;   // average wait time per operation, 
                             // i.e. time from submission to issue of the IO request to start of disk operation

int max_waittime = 0;        // maximum wait time for any io operation.

int io_num = 0;
int current_track = 0;
int current_time = 1;

void readInstruction(char* iFilename, IO* io) {
	ifstream input(iFilename, ios::in);
	string line;
	while(getline(input, line)) {
		if(line[0] == '#') continue;
		istringstream iss(line);
		int a[2];
		if(!(iss >> a[0] >> a[1])) {
			break;
		}
		int time = a[0];
		int track = a[1];
		Request* req = new Request(time, track);
		io->initialize(req);
	}
	input.close();
}

bool parseInput(int argc, char** argv, string& s) {
	int c;
	opterr = 0;
	while ((c = getopt(argc, argv, "s:")) != -1) {
		switch (c) {
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

IO* chooseAlgo(string s) {
	if(s.compare("i") == 0) {
		return new FIFO();
	}
	else if(s.compare("j") == 0) {
		return new SSTF();
	}
	else if(s.compare("s") == 0) {
		return new SCAN();
	}
	else if(s.compare("c") == 0) {
		return new CSCAN();
	}
	else if(s.compare("f") == 0) {
		return new FSCAN();
	}
	else {
		cout << "Wrong User Input Option!" << endl;
		exit(0);
	}
}

void print_summary() {
	total_time = current_time;
	avg_turnaround = double(tot_turnaround) / io_num;
	avg_waittime = double(tot_wait) / io_num;
	printf("SUM: %d %d %.2lf %.2lf %d\n",
		total_time,
		tot_movement,
		avg_turnaround,
		avg_waittime,
		max_waittime);
}

void simulation(IO* io, bool trace) {
	Request* req1;
	Request* req2;

	if(trace == true) {
		printf("TRACE\n");
	}
	while(io->has_unload_request() || io->has_wait_request()) {
		int track, time, num;
		if(io->has_unload_request()) {
			req1 = io->get_from_reqlist();
			track = req1->req_track;
			time = req1->req_start;
		}

		// an io request is arriving
		if(io->has_unload_request() && io->check_arriving(req1, current_time)) {
			// cout << "test 130" << endl;
			io->set_add(req1, current_time, io_num);
			if(trace) {
				printf("%d:     %d add %d\n", current_time, io_num, req1->req_track);
			}
			io->delete_from_req();
			io_num++;
			continue;
		}

		if(io->check_ongoing() && io->has_wait_request()) {
			// disk is working
			// check finished
			req2 = io->get_ongoing_req(); // change here, this method needs to be rewritten
			track = req2->req_track;
			time = req2->req_start;
			num = req2->req_num;
			if(io->check_completed(req2, current_track)) {
				io->set_finish(req2, current_time);
				if(trace) {
					printf("%d:     %d finish %d\n", current_time, req2->req_num, req2->req_turnaround);
				}
				tot_turnaround += req2->req_turnaround;
				io->clear_ongoing();
				io->delete_from_wait();
				continue;
			}
			else {
				if(track > current_track) {
					current_track++;

				}
				else {
					current_track--;
				}
				tot_movement++;
				req2->move_track++;
				current_time++;
				continue;
			}
		}
		else if(!io->check_ongoing() && io->has_wait_request()) {
			// an io request is going to be scheduled
			req2 = io->get_from_waitlist(current_track);
			io->set_ongoing();
			req2->req_wait = current_time - req2->req_start;
			max_waittime = max(req2->req_wait, max_waittime);
			tot_wait += req2->req_wait;
			if(trace) {
				printf("%d:     %d issue %d %d\n", current_time, req2->req_num, req2->req_track, current_track);
			}
			// io->set_issue(req2, current_track, current_time);
			continue;
		}
		else if(!io->check_ongoing() && !io->has_wait_request()){
			// no disk working, no request in the queue, just time passing
			current_time++;
			continue;
		}
	}
}

int main(int argc, char** argv) {
	IO* io_sched;
	algospec = "i"; // default algorithm
	trace = false; // trace 

	// user input options
	if(!parseInput(argc, argv, algospec)) {
		exit(0);
	}
	io_sched = chooseAlgo(algospec);
	readInstruction(argv[2], io_sched);
	simulation(io_sched, trace);
	print_summary();

	return 1;
}