all:
	g++ -c Start.cpp -o Start.o
	g++ -c DES_Layer.cpp -o DES_Layer.o
	g++ -c Process.cpp -o Process.o
	g++ -c Event.cpp -o Event.o
	g++ -c Scheduler.cpp -o Scheduler.o
	g++ -c FCFS.cpp -o FCFS.o
	g++ -c LCFS.cpp -o LCFS.o
	g++ -c SJF.cpp -o SJF.o
	g++ -c RR.cpp -o RR.o
	g++ -c PRIO.cpp -o PRIO.o
	g++ Start.o DES_Layer.o Process.o Event.o Scheduler.o FCFS.o LCFS.o SJF.o RR.o PRIO.o -o Start

clean:
	rm -rf *.o
	rm Start
