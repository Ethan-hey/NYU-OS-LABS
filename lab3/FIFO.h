#ifndef FIFO_H
#define FIFO_H

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

class FIFO : public Manager {
public:
	FIFO(int fsize, vector<int> rand, int count) : Manager(fsize, rand, count){};
	~FIFO(){};
	int get_replaced_frame();
	void print_pagetable();
	void print_frametable();
	void set_map(int vpage, int fpage);
	void set_loaded(int fpage);
	void set_class(int vpage);

	int frame_size;
	list<int> loaded;
};

#endif