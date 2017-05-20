#include "CLOCK_V.h"
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

void CLOCK_V::set_class(int vpage) {
	;
}

void CLOCK_V::set_loaded(int fpage) {
	;
}

int CLOCK_V::get_replaced_frame() {
	while(1) {
		if(check_presented(hand)) {
			if(check_referenced(hand)) {
				clear_reference(hand);
				hand = (hand + 1) % 64;
			}
			else {
				int result = hand;
				hand = (hand + 1) % 64;
				return ptlist[result] % 64; // get the last 6 bits
			}
		}
		else {
			hand = (hand + 1) % 64;
		}
	}
}

void CLOCK_V::print_frametable() {
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