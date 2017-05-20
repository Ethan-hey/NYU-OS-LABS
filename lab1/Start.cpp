/*
 * Start.cpp
 *
 *  Created on: 2017年2月17日
 *      Author: haoranma
 */

#include "Linker.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cctype>
using namespace std;

int linenum = 1;
int lineoffset = 1;
int prelinenum = 1;
int prelineoffset = 1;
int prelinelength = 1;
int totalindex = 1;

vector<Definition> defList;
vector<Use> useList;
vector<Instruction> instrList;
vector<Module> modList;

// regex test("[a-zA-Z][a-zA-Z0-9]*");
// regex test0("[0][0]*");

// parse error type
string errstr[] = {
		"NUM_EXPECTED", 				// index 0: Number expect
		"SYM_EXPECTED", 				// index 1: Symbol Expected
		"ADDR_EXPECTED",                // index 2: Addressing Expected which is A/E/I/R
		"SYM_TOO_LONG",                 // index 3: Symbol Name is to long
		"TO_MANY_DEF_IN_MODULE",        // index 4: > 16
		"TO_MANY_USE_IN_MODULE",        // index 5: > 16
		"TO_MANY_INSTR"                 // index 6: total num_instr exceeds memory size (512)
};

void parseError(int errcode) {
	static string errstr[] = {
	"NUM_EXPECTED",    					// Number expect
	"SYM_EXPECTED", 					// Symbol Expected
	"ADDR_EXPECTED", 					// Addressing Expected which is A/E/I/R
	"SYM_TOO_LONG", 					// Symbol Name is to long
	"TO_MANY_DEF_IN_MODULE", 			// > 16
	"TO_MANY_USE_IN_MODULE", 			// > 16
	"TO_MANY_INSTR" 					// total num_instr exceeds memory size (512)
	};
	cout << "Parse Error line " << prelinenum << " offset " << prelineoffset << ": " << errstr[errcode] << endl;
}

// Parse the token and locate parse error
string getToken(ifstream &input) {
	string read = "";
	int n = 0;
	int tokenLen = 0;
	while((n = input.get()) != -1) {
		char c = char(n);
		lineoffset++;
		totalindex++;
		if(tokenLen == 0) {
			if(c == ' ' || c == '\t') {
				continue;
			}
			else if(c == '\n') {
				prelinelength = lineoffset;
				lineoffset = 1;
				linenum++;
				continue;
			}
			else {
				tokenLen++;
				prelinenum = linenum;
				prelineoffset = lineoffset - 1;
				read += c;
				continue;
			}
		}
		else {
			if(c == ' ' ||  c == '\t') {
				tokenLen = 0;
				return read;
			}
			else if(c == '\n') {
				tokenLen = 0;
				prelinelength = lineoffset;
				lineoffset = 1;
				linenum++;
				return read;
			}
			else {
				tokenLen++;
				read += c;
			}
		}
	}
	// break at the end
	if(read.compare("") == 0) {
		if(lineoffset == 1 && linenum - prelinenum >= 1 && totalindex > 1) {
			prelineoffset = prelinelength - 1;
			prelinenum = linenum - 1;
		}
		else {
			prelinenum = linenum - 1;
		}
	}
}

bool isnumber(char c) {
	int n = (int)c;
	if(n >= 48 && n <= 57) {
		return true;
	}
	else return false;
}

bool isSymbol(string s) {
    if (s.compare("") == 0) {
        return false;
    } 
    int n = s.size();
    char temp[n];
    for(int i = 0; i < n; i++) {
    	temp[i] = s.c_str()[i];
    }
    char c = temp[0];
    if (!isalpha(c)) {
        return false;
    }
    for (int i = 1; i < n; i++) {
        c = temp[i];
        if (!isnumber(c) && !isalpha(c)) return false;
    }
    return true;		
}

int sameSymbol(string s) {
	for(int i = 0; i < defList.size(); i++) {
		if(defList.at(i).symbol.compare(s) == 0) {
			return i;
		}
	}
	return -1;
}

bool isNumber(string s) {
	if (s.compare("") == 0) {
        return false;
    } 
    int n = s.size();
    char temp[n];
    char c;
    for(int i = 0; i < n; i++) {
    	temp[i] = s.c_str()[i];
    }
    for (int i = 0; i < n; i++) {
    	c = temp[i];
        if (!isnumber(c)) return false;
    }   
	return true;
}

bool isType(string s) {
	if(s.compare("A") && s.compare("I") && s.compare("E") && s.compare("R")) {
		return false;
	}
	else return true;
}

// Pass one
void passOne(string name) {
	ifstream input(name, ios::in);
	int base = 0;
	int countInstr = 0; // global number of instructions
	string token;

	// parse def list
	while((token = getToken(input)) != "") {
		// parse numOfDef
		Module m;
		int numOfDef = atoi(token.c_str());
		// Parse error: number expected
		if(!isNumber(token)) {
			parseError(0);
			exit(1);
		}
		// Parse error: too many definitions
		if(numOfDef > 16) {
			parseError(4);
			exit(1);
		}
		for(int i = 0; i < numOfDef; i++) {
			// parese def
			token = getToken(input);
			if(!isSymbol(token)) {
				parseError(1);
				exit(1);
			}
			// Parse error: symbol name is too long
			if(token.length() > 16) {
				parseError(3);
				exit(1);
			}
			token = getToken(input);
			if(!isNumber(token)) {
				// Parse error: symbol expected
				parseError(0);
				exit(1);
			}
		}

	    // parse use list
	    token = getToken(input);
	    int numOfUse = atoi(token.c_str());
	    // Parse error: number expected
	    if(!isNumber(token)) {
	    	parseError(0);
	    	exit(1);
	    }
	    // Parse error: too many uses
	    if(numOfUse > 16) {
	    	parseError(5);
	    	exit(1);
	    }
	    for(int i = 0; i < numOfUse; i++) {
	    	// parse use
	    	token = getToken(input);
	    	if(!isSymbol(token)) {
				// Parse error: symbol expected
	    		parseError(1);
				exit(1);
			}
	    }

	    // parse instr list
	    token = getToken(input);
	    int numOfInstr = atoi(token.c_str());
	    // Parse error: number expected
	    if(!isNumber(token)) {
	    	parseError(0);
	    	exit(1);
	    }
	    countInstr = countInstr + numOfInstr;
	    // Parse error: too many instructions
	    if(countInstr > 512) {
	    	parseError(6);
	    	exit(1);
	    }
	    for(int i = 0; i < numOfInstr; i++) {
	    	// parse instr type
	    	token = getToken(input);
	    	// Parse error: address expected
	    	if(!isType(token)) {
	    		parseError(2);
	    		exit(1);
	    	}
	    	token = getToken(input);
	    }
	    m.base = base;
	    modList.push_back(m);
	    base += numOfInstr;
	}
}

// Pass two
void passTwo(string name) {
	ifstream input(name, ios::in);
	string token;
	int index = 0;

	// parse def list
	while((token = getToken(input)) != "" && index < modList.size()) {
		// initialization
		modList.at(index).num = index + 1;

		// parse numOfDef
		int numOfDef = atoi(token.c_str());
		for(int i = 0; i < numOfDef; i++) {
			// parese def
			string sym = getToken(input);
			string add = getToken(input);
			int address = atoi(add.c_str());
			Definition d(sym, address);
			d.ofModNum = modList.at(index).num;
			defList.push_back(d);
			modList.at(index).localdefList.push_back(d);
		}

		// parse use
	    token = getToken(input);
	    int numOfUse = atoi(token.c_str());
	    for(int i = 0; i < numOfUse; i++) {
	    	// parse use
	    	token = getToken(input);
	    	string sym = token;
	    	Use u(sym);
	    	u.ofModNum = index;
	    	useList.push_back(u);
	    	modList.at(index).localuseList.push_back(u);
	    }

	    // parse instr list
	    token = getToken(input);
	    int numOfInstr = atoi(token.c_str());
	    // cout << numOfInstr << endl;
	    modList.at(index).instrLen = numOfInstr;
	    for(int i = 0; i < numOfInstr; i++) {
	    	// parse and store instructions
	    	token = getToken(input);
	    	string type = token;
	    	token = getToken(input);
	    	string op = token;i
	    	int code = atoi(op.c_str());
	    	Instruction in(type, code);
	    	in.ofModNum = index;
	    	instrList.push_back(in);
	    	modList.at(index).localinstrList.push_back(in);
	    }
	    index++;
	}
}

// Set definition used or not
void setdefUsed() {
	for(int i = 0; i < defList.size(); i++) {
		for(int j = 0; j < useList.size(); j++) {
			if(defList.at(i).symbol.compare(useList.at(j).symbol) == 0) {
				defList.at(i).used = true;
			}
		}
	}
}

// Resolve Absolute address
void resolveA() {
	for(int i = 0; i < defList.size(); i++) {
		int temp = defList.at(i).ofModNum;
		defList.at(i).address += modList.at(temp - 1).base;
	}
}

// Resolve External address (before warning1)
void resolveE() {
	for(int i = 0; i < modList.size(); i++) {
		Module mtemp = modList.at(i);
		for(int j = 0; j < mtemp.localinstrList.size(); j++) {
			Instruction itemp = mtemp.localinstrList.at(j);
			if(itemp.type.compare("E") == 0) {
				int temp = itemp.operand;
				if(temp > mtemp.localuseList.size() - 1) {
					modList.at(i).localinstrList.at(j).errType = 3;
				}
				else {
					// Error: Exteranl used but no defined
					Use u = mtemp.localuseList.at(temp);
					int definedIndex = -1;
					bool defined = false;
					for(int k = 0; k < defList.size(); k++) {
						Definition d = defList.at(k);
						if(u.symbol.compare(d.symbol) == 0) {
							defined = true;
							definedIndex = k;
						}
					}
					if(defined)	{
						modList.at(i).localinstrList.at(j).op = defList.at(definedIndex).address + modList.at(i).localinstrList.at(j).opcode * 1000;
						modList.at(i).localinstrList.at(j).errType = 0;
					}
					else {
						modList.at(i).localinstrList.at(j).errType = 6;
						mtemp.localuseList.at(itemp.operand).noDef = true;
					}
				}
			}
		}
	}
}

// Solve things related to instructions other than type "E"
void solveInstr() {
	for(int i = 0; i < modList.size(); i++) {
		Module m = modList.at(i);
		for(int j = 0; j < m.localinstrList.size(); j++) {
			Instruction instr = m.localinstrList.at(j);
			if(instr.opcode < 10) {
				if(instr.type.compare("A") == 0 && instr.operand > 512) {
					modList.at(i).localinstrList.at(j).op = instr.opcode * 1000;
					modList.at(i).localinstrList.at(j).errType = 1;
				}
				if(instr.type.compare("R") == 0) {
					if(instr.operand > m.instrLen) {
						modList.at(i).localinstrList.at(j).op = m.base + instr.opcode * 1000;
						modList.at(i).localinstrList.at(j).errType = 2;
					}
					else {
						instr.operand += m.base;
						modList.at(i).localinstrList.at(j).op = instr.operand + instr.opcode * 1000;
						modList.at(i).localinstrList.at(j).errType = 0;
					}
				}
			}
			else {
				if(instr.type.compare("I") == 0) {
					modList.at(i).localinstrList.at(j).op = 9999;
					modList.at(i).localinstrList.at(j).errType = 4;
				}
				else {
					modList.at(i).localinstrList.at(j).op = 9999;
					modList.at(i).localinstrList.at(j).errType = 5;
				}
			}
		}
	}
}

// Check a symbol refer to type "E" is in the uselist or not 
void setuseUsed() {
	for(int i = 0; i < modList.size(); i++) {
		Module mtemp = modList.at(i);
		for(int j = 0; j < modList.at(i).localuseList.size(); j++) {
			Use utemp = mtemp.localuseList.at(j);
			for(int k = 0; k < mtemp.localinstrList.size(); k++) {
				Instruction itemp = mtemp.localinstrList.at(k);
				if(!(itemp.type == "E" && j == itemp.operand)) {
					continue;
				}
				else {
					modList.at(i).localuseList.at(j).used = true;
					break;
				}
				if(k == mtemp.localinstrList.size() - 1) {
					modList.at(i).localuseList.at(j).used = false;
				}
			}
		}
	}
}

// Check multiple definition
void multiDef() {
	if(defList.size() != 0) {
		for(int i = 0; i < defList.size() - 1; i++) {
			for(int j = i + 1; j < defList.size(); j++) {
				if(defList.at(i).symbol.compare(defList.at(j).symbol) == 0) {
					defList.at(j).address = defList.at(i).address;
					defList.at(i).multidef = true;
					defList.at(j).multidef = true;
					defList.at(i).printMultiDef = true;
				}
			}
		}
	}
}

// Print symbol table
void printSymbolTable() {
	for(int i = 0; i < defList.size(); i++) {
		int index = defList.at(i).ofModNum - 1;
		if(defList.at(i).address > modList.at(index).base + modList.at(index).instrLen - 1) {
			int temp = defList.at(i).address;
			defList.at(i).address = 0;
			defList.at(i).changed = true;
			cout << "Warning: Module "
					<< defList.at(i).ofModNum
					<< ": "
					<< defList.at(i).symbol
					<< " too big "
					<< temp
					<< " (max="
					<< modList.at(index).base + modList.at(index).instrLen - 1
					<< ")"
					<< " assume zero relative" << endl;
		}
	}
	cout << "Symbol Table" << endl;
	for(int i = 0; i < defList.size(); i++) {
		if(defList.at(i).multidef) {
			if(defList.at(i).printMultiDef) {
				cout << defList.at(i).symbol << "=" << defList.at(i).address << " Error: This variable is multiple times defined; first value used" << endl;
			}
			else continue;
		}
		else {
			cout << defList.at(i).symbol << "=" << defList.at(i).address << endl;
		}
	}
	cout << endl;
}

// Solve things related to warning change
void changeByWarning() {
	for(int i = 0; i < defList.size(); i++) {
		if(defList.at(i).changed == true) {
			int modnum = defList.at(i).ofModNum;
			Module mtemp = modList.at(modnum - 1);
			for(int j = 0; j < mtemp.localinstrList.size(); j++) {
				if(mtemp.localinstrList.at(j).type.compare("E") == 0) {
					modList.at(modnum - 1).localinstrList.at(j).op = mtemp.localinstrList.at(j).opcode * 1000;
				}
				else continue;
			}
		}
	}
}

// Print memory map
void printMemoryMap() {
	cout << "Memory Map" << endl;
	int instrIndex = 0;
	for(int i = 0; i < modList.size(); i++) {
		Module m = modList.at(i);
		for(int j = 0; j < m.instrLen; j++) {
			Instruction instr = m.localinstrList.at(j);
			switch(instr.errType) {
				case 0: {
					printf("%03d: %04d\n", instrIndex++, instr.op);
					break;
				}
				case 1: {
					printf("%03d: %04d", instrIndex++, instr.op);
					cout << " Error: Absolute address exceeds machine size; zero used" << endl;
					break;
				}
				case 2: {
					printf("%03d: %04d", instrIndex++, instr.op);
					cout << " Error: Relative address exceeds module size; zero used" << endl;
					break;
				}
				case 3: {
					printf("%03d: %04d", instrIndex++, instr.op);
					cout << " Error: External address exceeds length of uselist; treated as immediate" << endl;
					break;
				}
				case 4: {
					printf("%03d: %04d", instrIndex++, instr.op);
					cout << " Error: Illegal immediate value; treated as 9999" << endl;
					break;
				}
				case 5: {
					printf("%03d: %04d", instrIndex++, instr.op);
					cout << " Error: Illegal opcode; treated as 9999" << endl;
					break;
				}
				case 6: {
					printf("%03d: %04d", instrIndex++, instr.op);
					cout << " Error: "
						 << m.localuseList.at(instr.operand).symbol
						 << " is not defined; zero used"
						 << endl;
				}
			}
		}
		for(int k = 0; k < modList.at(i).localuseList.size(); k++) {
			if(!modList.at(i).localuseList.at(k).used) {
				cout << "Warning: Module "
					 << i + 1
					 << ": "
					 << modList.at(i).localuseList.at(k).symbol
					 << " appeared in the uselist but was not actually used"
					 << endl;
			}
		}
	}
	cout << endl;
}

// Print other warnings
void printDefNoUse() {
	for(int i = 0; i < defList.size(); i++) {
		if(defList.at(i).used == false) {
			printf("Warning: Module %d: %s was defined but never used\n", defList.at(i).ofModNum, defList.at(i).symbol.c_str());
		}
	}
}


int main(int argc, char* argv[]) {
	bool test = false;
	if(test) {
		passOne(argv[1]);
		passTwo(argv[1]);
		resolveA();
		multiDef();
		resolveE();
		printSymbolTable();
		changeByWarning();
		setuseUsed();
		setdefUsed();
		solveInstr();
		printMemoryMap();
		printDefNoUse();
	}
	else {
		passOne("/Users/haoranma/Desktop/haoran-os-lab1/input-1");
		passTwo("/Users/haoranma/Desktop/haoran-os-lab1/input-1");
		resolveA();
		multiDef();
		resolveE();
		printSymbolTable();
		changeByWarning();
		setuseUsed();
		setdefUsed();
		solveInstr();
		printMemoryMap();
		printDefNoUse();
	}
	return 1;
}






