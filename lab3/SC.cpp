#include "SC.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <list>
#include <limits.h>

using namespace std;

void SC::set_loaded(int frame) {
	loaded.push_back(frame);
}

void SC::set_class(int vpage) {
	;
}

int SC::get_replaced_frame() {
	while(1) {
		int frame = loaded.front();
		if(check_referenced(ftlist[frame])) {
			clear_reference(ftlist[frame]);
			loaded.push_back(frame);
			loaded.pop_front();
		}
		else {
			loaded.pop_front();
			return frame;
		}
	}
}

void SC::print_frametable() {
	for(int i = 0; i < frame_size; i++) {
		if(ftlist[i] >= 0) {
			cout << ftlist[i] << " ";
		}
		else {
			cout << "* ";
		}
	}
	cout << " " << "|| ";
	for(list<int> :: iterator it = loaded.begin(); it != loaded.end();) {
		int fpage = (*it);
		cout << fpage << " ";
		it++;
	}
	cout << endl;
}