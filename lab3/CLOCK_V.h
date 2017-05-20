#ifndef CLOCK_V_H
#define CLOCK_V_H

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

class CLOCK_V : public Manager {
public:
	CLOCK_V(int fsize, vector<int> rand, int count) : Manager(fsize, rand, count){};
	~CLOCK_V(){};
	
	int get_replaced_frame();
	void print_frametable();
	void set_map(int vpage, int fpage);
	void set_loaded(int fpage);
	void set_class(int vpage);
};

#endif