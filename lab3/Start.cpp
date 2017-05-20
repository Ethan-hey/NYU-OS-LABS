#include "Manager.h"
#include "Instruction.h"
#include "Random.h"
#include "FIFO.h"
#include "SC.h"
#include "AGING_P.h"
#include "AGING_V.h"
#include "CLOCK_P.h"
#include "CLOCK_V.h"
#include "NRU.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <unistd.h>
#include <limits.h>
#include <algorithm>

using namespace std;

vector<int> randArray;
vector<Instruction> instrList;
int ofs = 0;
int countRand = 0;
int algo = -1;
string algospec;
string option;
int frame_size = 32;

// statistics
int inst_count = 0;
int unmaps = 0;
int maps = 0;
int ins = 0;
int outs = 0;
int zeros = 0;
long long totalcost = 0;

// read random numbers
void readRandom(char* rfilename) {
	ifstream rFile(rfilename, ios::in);
	string line;
	getline(rFile, line);
	countRand = atoi(line.c_str());
	for (int i = 0; i < countRand; i++) {
		getline(rFile, line);
		randArray.push_back(atoi(line.c_str()));
	}
	rFile.close();
}

void readInstruction(char* iFilename) {
	ifstream input(iFilename, ios::in);
	string line;
	while(getline(input, line)) {
		if(line[0] == '#') continue;
		istringstream iss(line);
		int a[2];
		if(!(iss >> a[0] >> a[1])) {
			break;
		}
		int type = a[0];
		int page = a[1];
		Instruction instr(type, page);
		instrList.push_back(instr);
	}
	input.close();
}

/* create random number
int myRandom(int num) {
	int temp = 0;
	if (ofs < countRand) {
		temp = ofs;
		ofs++;
		return randArray[temp] % num;
	}
	else {
		temp = ofs - countRand;
		ofs++;
		return randArray[temp] % num;
	}
}*/

bool parseInput(int argc, char** argv) {
	int c;
	opterr = 0;
	vector<string> input;

	for(int i = 0; i < argc; i++) {
		input.push_back(string(argv[i]));
	}
	for(int i = 0; i < argc; i++) {
		if(input[i].find("-a") != string::npos) {
			algospec = input[i].substr(2);
		}
		if(input[i].find("-o") != string::npos) {
			option = input[i].substr(2);
		}
		if(input[i].find("-f") != string::npos) {
			frame_size = atoi(input[i].substr(2).c_str());
		}
	}
	/*while ((c = getopt(argc, argv, "a:")) != -1) {
		switch (c) {
		case 'a': {
			cout << optarg << endl; 
			algospec = optarg;
			break;
		}
		case 'o': {
			option = optarg;
			break;
		}
		case 'f': {
			frame_size_temp = optarg;
			frame_size = atoi(frame_size_temp.c_str());
			break;
		}
		default: {
			abort();
		}
		}
	}
	printf("algospec = %s, option = %s, frame_size = %d\n", algospec.c_str(), option.c_str(), frame_size);
	for(int index = optind; index < argc; index++) {
		printf ("Non-option argument %s\n", argv[index]);
	}*/
	return true;
}

Manager* chooseAlgo(string a, int frame_size) {
	if(a.compare("r") == 0) {
		algo = 1;
		return new RANDOM(frame_size, randArray, countRand);
	}
	else if(a.compare("f") == 0) {
		algo = 2;
		return new FIFO(frame_size, randArray, countRand);
	}
	else if(a.compare("s") == 0) {
		algo = 3;
		return new SC(frame_size, randArray, countRand);
	}
	else if(a.compare("c") == 0) {
		algo = 4;
		return new CLOCK_P(frame_size, randArray, countRand);
	}
	else if(a.compare("a") == 0) {
		algo = 5;
		return new AGING_P(frame_size, randArray, countRand);
	}
	else if(a.compare("N") == 0) {
		algo = 6;
		return new NRU(frame_size, randArray, countRand);
	}
	else if(a.compare("X") == 0) {
		algo = 7;
		return new CLOCK_V(frame_size, randArray, countRand);
	}
	else if(a.compare("Y") == 0) {
		algo = 8;
		return new AGING_V(frame_size, randArray, countRand);
	}
	else {
		cout << "Wrong User Input Option!" << endl;
		exit(0);
	}
}

void print_summary() {
	totalcost = long(400 * (maps + unmaps)) + long(3000 * (ins + outs)) + long(150 * zeros + inst_count);
	printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu\n",
		inst_count,
		unmaps,
		maps,
		ins, 
		outs, 
		zeros, 
		totalcost);
}

void simulation(Manager* m, int size) {
	int instr_index = 0; 

	m->hand = 0; // for clock algorithms, hand initializes to zero
	while(instr_index < instrList.size()) {
		Instruction instr = instrList[instr_index];
		int instr_type = instr.rw_type;
		int instr_page = instr.virtual_page;

		if(instr_type == 1) {
			m->set_modify(instr_page);
		}
		
		// check the page: If it is not presented, locate a free frame,
		// if the frame is paged out, page in, else zero the frame;
		// If it is presented, implement replacement algorithms
		if(option.find("O") != string::npos) {
			m->print_instr(instr);
		}
		if(!m->check_presented(instr_page)) {
			// not presented
			int newframe = m->get_free_frame(size);
			if(newframe == -1) {
				// replacement algorithms
				newframe = m->get_replaced_frame();
				int oldpage = m->get_old_page(newframe);
				m->unmap_old_map(oldpage, newframe);
				if(option.find("O") != string::npos) {
					printf("%d: UNMAP %3d %3d\n", instr_index, oldpage, newframe);
				}
				unmaps++;
				m->clear_present(oldpage);
				if(m->check_dirty_page(oldpage)) {
					m->set_pageout(oldpage);
					if(option.find("O") != string::npos) {
						printf("%d: OUT %5d %3d\n", instr_index, oldpage, newframe);
					}
					outs++;
				}
				m->clear_reference(oldpage);
				m->clear_modify(oldpage);
			}
			m->set_map(instr_page, newframe);
			m->set_loaded(newframe);
			maps++;
			if(m->check_pagedout(instr_page)) {
				if(option.find("O") != string::npos) {
					printf("%d: IN %6d %3d\n", instr_index, instr_page, newframe);
				}
				ins++;
			}
			else {
				if(option.find("O") != string::npos) {
					printf("%d: ZERO %8d\n", instr_index, newframe);
				}
				zeros++;
			}
			m->set_present(instr_page);
			m->set_reference(instr_page);
			if(option.find("O") != string::npos) {
				printf("%d: MAP %5d %3d\n", instr_index, instr_page, newframe);
			}
			if(option.find("p") != string::npos) {
				m->print_pagetable();
			}
			if(option.find("f") != string::npos) {
				m->print_frametable();
			}
			if(option.find("a") != string::npos) {
				m->print_aging();
			}
		}
		else {
			// cout << "presented" << endl;
			m->set_reference(instr_page);
			if(option.find("p") != string::npos) {
				m->print_pagetable();
			}
			if(option.find("f") != string::npos) {
				m->print_frametable();
			}
			if(option.find("a") != string::npos) {
				m->print_aging();
			}
		}
		instr_index++;
	}
	inst_count = instr_index;
	if(option.find("P") != string::npos) {
		m->print_pagetable();
	}
	if(option.find("F") != string::npos) {
		m->print_final_frametable();
	}
	if(option.find("S") != string::npos) {
		print_summary();
	}
}

int main(int argc, char** argv) {
	Manager* mmu;
	algospec = "r"; // default algorithm
	option = "OPFS"; // default operation
	frame_size = 32; // default size of frames

	// user input options
	if(!parseInput(argc, argv)) {
		exit(0);
	}
	readRandom(argv[argc - 1]);
	mmu = chooseAlgo(algospec, frame_size);
	// cout << "test 305" << mmu->index0 << endl;
	readInstruction(argv[argc - 2]);
	mmu->initialize_pagetable_entry();
	mmu->initialize_frametable_entry(frame_size);
	simulation(mmu, frame_size);

	return 1;
}