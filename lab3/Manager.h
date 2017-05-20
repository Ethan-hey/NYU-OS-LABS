#ifndef MANAGER_H
#define MANAGER_H

#include "Instruction.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <list>
#include <unistd.h>
#include <limits.h>
#include <algorithm>

using namespace std;

class Manager {
public:
	Manager(int fsize, vector<int> rand, int count);
	~Manager(){};
	int ptlist[64];
	// From left to right
	// 1th bit: PRESENTED
	// 2th bit: MODIFIED
	// 3th bit: REFERENCED
	// 4th bit: PAGEOUT
	// 5-10th bit: FRAMEINDEX
	int ftlist[64]; // store the virtual page number it has mapped

	int hand;
	vector<int> randArray;
	int frame_size;
	int ofs;
	int countRand;

	void initialize_pagetable_entry();
	void initialize_frametable_entry(int fsize);

	void print_instr(Instruction instr);

	// set
	void set_present(int vpage);
	void set_modify(int vpage);
	void set_reference(int vpage);
	void set_pageout(int vpage);

	// clear
	void clear_present(int vpage);
	void clear_modify(int vpage);
	void clear_reference(int vpage);

	// check
	bool check_presented(int vpage);
	bool check_dirty_page(int vpage);
	bool check_referenced(int vpage);
	bool check_pagedout(int vpage);

	// get or replace
	int get_free_frame(int framesize);
	void set_map(int vpage, int fpage);
	int get_old_page(int fpage);
	void unmap_old_map(int vpage, int fpage);

	// print transitions
	virtual void set_loaded(int fpage) = 0; // for FIFO, SC
	virtual void set_class(int vpage) = 0; // for NRU
	// for all
	virtual int get_replaced_frame() = 0;
	virtual void print_frametable() = 0;

	void print_pagetable();
	void print_final_frametable();
	void print_aging();
};

#endif