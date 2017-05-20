#include "AGING_P.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <list>

using namespace std;

void AGING_P::set_class(int vpage) {
	;
}

void AGING_P::set_loaded(int fpage) {
	;
}

void AGING_P::update_aging() {
	for(int i = 0; i < frame_size; i++) {
		aging_vec[i] = (aging_vec[i] >> 1);
		int vpage = ftlist[i];
		if(check_referenced(vpage)) {
			aging_vec[i] |= 0x80000000;
		}
	}
}

int AGING_P::find_min() {
	int result = -1;
	unsigned int min = aging_vec[0];
	for(int i = 1; i < frame_size; i++) {
		if(aging_vec[i] < min && aging_vec[i] != 0) {
			result = i;
			min = aging_vec[i];
		}
	}
	if(result == -1) {
		aging_vec[0] = 0;
		return 0;
	}
	else {
		aging_vec[result] = 0;
		return result;
	}
}

int AGING_P::get_replaced_frame() {
	update_aging();
	int frame = find_min();
	for(int i = 0; i < 64; i++) {
		if(check_presented(i)) {
			clear_reference(i);
		}
	}
	return frame;
}

void AGING_P::print_frametable() {
	for(int i = 0; i < frame_size; i++) {
		if(ftlist[i] >= 0) {
			cout << ftlist[i] << " ";
		}
		else {
			cout << "* ";
		}
	}
	cout << " || ";
	for(int i = 0; i < frame_size; i++) {
		if(ftlist[i] >= 0) {
			cout << i << ":" << hex << aging_vec[i] << dec << " ";
		}
	}
	cout << endl;
}