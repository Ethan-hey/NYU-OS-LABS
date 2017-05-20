all:
	g++ -c Start.cpp -o Start.o
	g++ -c Manager.cpp -o Manager.o
	g++ -c Instruction.cpp -o Instruction.o
	g++ -c Random.cpp -o Random.o
	g++ -c FIFO.cpp -o FIFO.o
	g++ -c AGING_P.cpp -o AGING_P.o
	g++ -c AGING_V.cpp -o AGING_V.o
	g++ -c CLOCK_P.cpp -o CLOCK_P.o
	g++ -c CLOCK_V.cpp -o CLOCK_V.o
	g++ -c NRU.cpp -o NRU.o
	g++ -c SC.cpp -o SC.o
	g++ Start.o Manager.o Instruction.o Random.o FIFO.o AGING_P.o AGING_V.o CLOCK_P.o CLOCK_V.o NRU.o SC.o -o Start

clean:
	rm -rf *.o
	rm Start
