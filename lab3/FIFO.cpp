#include "FIFO.h"
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

void FIFO::set_loaded(int fpage) {
	loaded.push_back(fpage);
}

int FIFO::get_replaced_frame() {
	int temp = loaded.front();
	loaded.pop_front();
	return temp;
}

void FIFO::set_class(int vpage) {
	
}

void FIFO::print_frametable() {
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