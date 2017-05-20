#include "Instruction.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <unistd.h>
#include <limits.h>

using namespace std;

Instruction::Instruction(int type, int page) {
	rw_type = type;
	virtual_page = page;
}