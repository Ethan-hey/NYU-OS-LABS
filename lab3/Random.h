#ifndef RANDOM_H
#define RANDOM_H

#include "Manager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class RANDOM : public Manager {
public:
	RANDOM(int fsize, vector<int> rand, int count) : Manager(fsize, rand, count){};
	~RANDOM(){};

	int get_replaced_frame();
	int myRandom(int num);
	void print_pagetable();
	void print_frametable();
	void set_class(int vpage);
	void set_loaded(int frame);
};

#endif