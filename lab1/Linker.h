/*
 * Linker.h
 *
 *  Created on: 2017年2月18日
 *      Author: haoranma
 */

 

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Definition {
private:

public:
	string symbol;
	int address;
	int ofModNum;
	bool multidef;
	bool used;
	bool printMultiDef;
	bool changed;

	Definition(string s, int n);
};

class Use {
private:

public:
	string symbol;
	bool used;
	bool noDef;
	int ofModNum;
	bool noE;
	Use(string s);
};

class Instruction {

public:
	string type;
	int opcode;
	int operand;
	int op;
	int ofModNum;
	int errType;

	Instruction(string instr, int code);
};

class Module {
private:
public:
	Module();
	int num;
	int base;
	int instrLen;
	int undefIndex;
	vector<Definition> localdefList;
	vector<Use> localuseList;
	vector<Instruction> localinstrList;

	void setInstrLen();
};

#endif /* LINKER_H_ */



