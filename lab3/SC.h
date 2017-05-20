#ifndef SC_H
#define SC_H

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

class SC : public Manager {
public:
	SC(int fsize, vector<int> rand, int count) : Manager(fsize, rand, count){};
	~SC(){};
	void print_pagetable();
	void print_frametable();
	void set_map(int vpage, int fpage);
	void set_loaded(int frame);
	void set_class(int vpage);
	int get_replaced_frame();
	list<int> loaded;
};

#endif