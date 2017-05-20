/*
 * Definition.cpp
 *
 *  Created on: 2017年2月18日
 *      Author: haoranma
 */

#include "Linker.h"
#include <string>
#include <iostream>
using namespace std;

Definition::Definition(string s, int a) {
	symbol = s;
	address = a;
	ofModNum = 0;
	used = false;
	multidef = false;
	printMultiDef = false;
	changed = false;
}






