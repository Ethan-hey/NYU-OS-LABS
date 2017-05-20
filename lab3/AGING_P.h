#ifndef AGING_P_H
#define AGING_P_H

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

using namespace std;

class AGING_P : public Manager {
public:
	AGING_P(int fsize, vector<int> rand, int count) : Manager(fsize, rand, count){
		for(int i = 0; i < frame_size; i++) {
			aging_vec[i] = 0;
		}
	};
	~AGING_P(){};

	int get_replaced_frame();
	void print_pagetable();
	void print_frametable();
	void set_class(int vpage);
	void set_loaded(int fpage);
	void update_aging();
	int find_min();

	unsigned int aging_vec[64];
};

#endif