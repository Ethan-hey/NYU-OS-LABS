#ifndef INSTRUCTION_H
#define INSTRUCTION_H

class Instruction {
public:
	int rw_type;
	int virtual_page;
	Instruction(int, int);
	Instruction(){};
	~Instruction(){};
};

#endif