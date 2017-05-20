#include "AGING_V.h"
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

void AGING_V::set_class(int vpage) {
	;
}

void AGING_V::set_loaded(int fpage) {
	;
}

void AGING_V::update_aging() {
    for(int i = 0; i < 64; i++) {
        if(check_presented(i)) {
            aging_vec[i] >>= 1;
            if(check_referenced(i)) {
                aging_vec[i] += 0x80000000;
            }
        }
    }
}

int AGING_V::find_min() {
	unsigned int minaging = 0x80000000;
    int index = -1;
    int mindex = 64;
    for(int i = 63; i >= 0; i--) {
        if(check_presented(i)) {
            mindex = min(mindex, i);
            if(minaging >= aging_vec[i] && aging_vec[i] != 0) {
                index = i;
                minaging = aging_vec[i];
            }
        }
    }
    if(index == -1) {
    	index = mindex;
    }

    aging_vec[index] = 0;

    return index;
}

int AGING_V::get_replaced_frame() {
	update_aging();
	int frame = find_min();
    for(int i = 0; i < 64; i++) {
        if(check_presented(i)) {
            clear_reference(i);
        }
    }
    return ptlist[frame] % 64;
}

void AGING_V::print_frametable() {
	for(int i = 0; i < frame_size; i++) {
		if(ftlist[i] >= 0) {
			cout << ftlist[i] << " ";
		}
		else {
			cout << "* ";
		}
	}
	cout << " || ";
	for(int i = 0; i < 64; i++) {
		if(check_presented(i)) {
			cout << i << ":" << hex << aging_vec[i] << dec << " ";
		}
	}
	cout << endl;
}