#include "NRU.h"
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

int NRU::myRandom(int num) {
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

void NRU::set_loaded(int frame) {
	;
}

void NRU::add_class(int vpage, int type) {
	if(type == 0) {
		class0[index0] = vpage;
		index0++;
	}
	else if(type == 1) {
		class1[index1] = vpage;
		index1++;
	}
	else if(type == 2) {
		class2[index2] = vpage;
		index2++;
	}
	else {
		class3[index3] = vpage;
		index3++;
	}
}

void NRU::set_class(int vpage) {
	if(check_dirty_page(vpage) && check_referenced(vpage)) {
		// add to class 3: referenced and modified
		add_class(vpage, 3);
	}
	else if(!check_dirty_page(vpage) && check_referenced(vpage)) {
		// add to class 2: referenced not modified
		add_class(vpage, 2);
	}
	else if(check_dirty_page(vpage) && !check_referenced(vpage)) {
		// add to class 1: not referenced but modified
		add_class(vpage, 1);
	}
	else if(!check_dirty_page(vpage) && !check_referenced(vpage)) {
		// add to class 0: not referenced not modified
		add_class(vpage, 0);
	}
}

void NRU::initial_class_index() {
	index0 = 0;
	index1 = 0;
	index2 = 0;
	index3 = 0;
}

int NRU::get_replaced_frame() {
	initial_class_index();
	for(int i = 0; i < 64; i++) {
		if(check_presented(i)) {
			set_class(i);
		}
	}
	int frame;
	if(index0 != 0){
        int result = myRandom(index0);
        frame = ptlist[class0[result]] % 64;
    }
    else if(index1 != 0){
    	int result = myRandom(index1);
        frame = ptlist[class1[result]] % 64;
    }
    else if(index2 != 0){
        int result = myRandom(index2);
        frame = ptlist[class2[result]] % 64;
    }
    else {
    	int result = myRandom(index3);
    	frame = ptlist[class3[result]] % 64;
    }
    clock++;
    if(clock == 10) {
    	clock = 0;
    	for(int i = 0; i < 64; i++) {
    		if(check_presented(i) > 0) {
    			clear_reference(i);
    		} 
    	}
    }
    return frame;
}

void NRU::print_frametable() {
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