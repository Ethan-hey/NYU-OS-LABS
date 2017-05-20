#include "Instruction.h"
#include "Manager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

Manager::Manager(int fsize, vector<int> rand, int count) {
	frame_size = fsize;
	randArray = rand;
	countRand = count;
	ofs = 0;
	hand = 0;
}

// From left to right
// 1th bit: PRESENTED
// 2th bit: MODIFIED
// 3th bit: REFERENCED
// 4th bit: PAGEOUT
// 5-10th bit: FRAMEINDEX 0 - 63

void Manager::initialize_pagetable_entry() {
	for(int i = 0; i < 64; i++) {
		ptlist[i] = 0;
	}
}

void Manager::initialize_frametable_entry(int framesize) {
	for(int i = 0; i < framesize; i++) {
		ftlist[i] = -1; // no mapping at first
	}
}

void Manager::print_instr(Instruction instr) {
	cout << "==> inst: " << instr.rw_type << " " << instr.virtual_page << endl;
}

// set

void Manager::set_present(int vpage) {
	ptlist[vpage] |= (1 << 10);
}

void Manager::set_modify(int vpage) {
	ptlist[vpage] |= (1 << 9);
}

void Manager::set_reference(int vpage) {
	ptlist[vpage] |= (1 << 8);
}

void Manager::set_pageout(int vpage) {
	ptlist[vpage] |= (1 << 7);
}

// clear

void Manager::clear_present(int vpage) {
	ptlist[vpage] &= ~ (1 << 10);
}

void Manager::clear_modify(int vpage) {
	ptlist[vpage] &= ~ (1 << 9);
}

void Manager::clear_reference(int vpage) {
	ptlist[vpage] &= ~ (1 << 8);
}

// check

bool Manager::check_presented(int vpage) {
	return ptlist[vpage] & 1 << 10;
}

bool Manager::check_dirty_page(int vpage) {
	return ptlist[vpage] & 1 << 9;
}

bool Manager::check_referenced(int vpage) {
	return ptlist[vpage] & 1 << 8;
}

bool Manager::check_pagedout(int vpage) {
	return ptlist[vpage] & 1 << 7;
}

//

int Manager::get_free_frame(int framesize) {
	int counter;
	for(counter = 0; counter < framesize; counter++) {
		if(ftlist[counter] == -1) {
			return counter;
		}
	}
	return -1;
}

int Manager::get_old_page(int fpage) {
	return ftlist[fpage];
}

void Manager::set_map(int vpage, int fpage) {
	// the last bits (in the right) are used to store frame index
	ptlist[vpage] += fpage;
	ftlist[fpage] = vpage;
}

void Manager::unmap_old_map(int vpage, int fpage) {
	ptlist[vpage] -= fpage;
	ftlist[fpage] = -1;
}

void Manager::print_pagetable() {
	for(int i = 0; i < 64; i++) {
		if(check_presented(i)) {
			cout << i << ":";
			if(check_referenced(i)) {
				cout << "R";
			}
			else {
				cout << "-";
			}
			if(check_dirty_page(i)) {
				cout << "M";
			}
			else {
				cout << "-";
			}
			if(check_pagedout(i)) {
				cout << "S";
			}
			else {
				cout << "-";
			}
			cout << " ";
		}
		else {
			if(check_pagedout(i)) {
				cout << "# ";
			}
			else {
				cout << "* ";
			}
		}
	}
	cout << endl;
}

void Manager::print_final_frametable() {
	for(int i = 0; i < frame_size; i++) {
		if(ftlist[i] >= 0) {
			cout << ftlist[i] << " ";
		}
		else {
			cout << "* ";
		}
	}
	cout << endl;
}

void Manager::print_aging() {

}
