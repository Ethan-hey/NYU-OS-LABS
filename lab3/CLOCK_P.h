#ifndef CLOCK_P_H
#define CLOCK_P_H

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

class CLOCK_P : public Manager {
public:
	CLOCK_P(int fsize, vector<int> rand, int count) : Manager(fsize, rand, count){};
	~CLOCK_P(){};
	int get_replaced_frame();
	void print_pagetable();
	void print_frametable();
	void set_class(int vpage);
	void set_loaded(int fpage);
};

#endif