#include "CLOCK_P.h"
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

void CLOCK_P::set_class(int vpage) {
	;
}

void CLOCK_P::set_loaded(int fpage) {
	;
}

int CLOCK_P::get_replaced_frame() {
	while(1) {
		int vpage = ftlist[hand];
		if(check_referenced(vpage)) {
			clear_reference(vpage);
			hand = (hand + 1) % frame_size;
		}
		else {
			int result = hand;
			hand = (hand + 1) % frame_size;
			return result;
		}
	}
}

void CLOCK_P::print_frametable() {
	for(int i = 0; i < frame_size; i++) {
		if(ftlist[i] >= 0) {
			cout << ftlist[i] << " ";
		}
		else {
			cout << "* ";
		}
	}
	cout << " || hand = " << hand << endl;
}