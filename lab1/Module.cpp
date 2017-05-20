/*
 * Module.cpp
 *
 *  Created on: 2017年2月18日
 *      Author: haoranma
 */

#include "Linker.h"
#include <string>
#include <iostream>
using namespace std;

Module::Module() {
	num = 0;
	base = 0;
	instrLen = 0;
	undefIndex = -1;
}

void Module::setInstrLen() {
	instrLen = localinstrList.size();
}



