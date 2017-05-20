#ifndef AGING_V_H
#define AGING_V_H

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

class AGING_V : public Manager {
public:
	AGING_V(int fsize, vector<int> rand, int count) : Manager(fsize, rand, count){
		for(int i = 0; i < frame_size; i++) {
			aging_vec[i] = 0;
		}
	};
	~AGING_V(){};

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