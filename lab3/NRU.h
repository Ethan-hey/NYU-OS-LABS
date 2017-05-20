#ifndef NRU_H
#define NRU_H

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

class NRU : public Manager {
public:
	NRU(int fsize, vector<int> rand, int count) : Manager(fsize, rand, count){
		index0 = 0;
		index1 = 0;
		index2 = 0;
		index3 = 0;
		clock = 0;
	};
	~NRU(){};
	int myRandom(int num);
	int get_replaced_frame();
	void print_frametable();

	void set_class(int vpage);
	void add_class(int vpage, int type);
	void initial_class_index();
	void set_loaded(int fpage);

	int index0, index1, index2, index3;
	int clock;
	int class0[64];
	int class1[64];
	int class2[64];
	int class3[64];
};

#endif