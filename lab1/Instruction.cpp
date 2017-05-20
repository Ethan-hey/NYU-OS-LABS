/*
 * Instruction.cpp
 *
 *  Created on: 2017年2月18日
 *      Author: haoranma
 */

#include "Linker.h"
#include <string>
#include <iostream>
using namespace std;

Instruction::Instruction(string s, int code) {
	type = s;
	op = code;
	operand = code % 1000;
	opcode = code / 1000;
	errType = 0;
}



