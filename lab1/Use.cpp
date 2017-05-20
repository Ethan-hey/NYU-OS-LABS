/*
 * Use.cpp
 *
 *  Created on: 2017年2月18日
 *      Author: haoranma
 */

#include "Linker.h"
#include <string>
#include <iostream>
using namespace std;

Use::Use(string s) {
	symbol = s;
	used = false;
	noDef = false;
	noE = false;
}




