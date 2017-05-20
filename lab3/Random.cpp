#include "Random.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <limits.h>

using namespace std;

int RANDOM::myRandom(int num) {
	int temp = 0;
	if (ofs < countRand) {
		temp = ofs;
		ofs++;
		return randArray[temp] % num;
	}
	else {
		ofs = 0;
		temp = ofs;
		ofs++;
		return randArray[temp] % num;
	}
}

void RANDOM::set_class(int vpage) {
	;
}

void RANDOM::set_loaded(int frame) {
	;
}

int RANDOM::get_replaced_frame() {
	return myRandom(frame_size);
}

void RANDOM::print_frametable() {
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