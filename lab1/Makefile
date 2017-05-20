all:
	g++ -std=c++0x -c Start.cpp -o Start.o
	g++ -std=c++0x -c Definition.cpp -o Definition.o
	g++ -std=c++0x -c Use.cpp -o Use.o
	g++ -std=c++0x -c Instruction.cpp -o Instruction.o
	g++ -std=c++0x -c Module.cpp -o Module.o
	g++ Start.o Definition.o Use.o Instruction.o Module.o -o Start

clean:
	rm -rf *.o
	rm Start
