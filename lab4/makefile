all:
	g++ -c Start.cpp -o Start.o
	g++ -c Request.cpp -o Request.o
	g++ -c IO.cpp -o IO.o
	g++ -c FIFO.cpp -o FIFO.o
	g++ -c SSTF.cpp -o SSTF.o
	g++ -c SCAN.cpp -o SCAN.o
	g++ -c CSCAN.cpp -o CSCAN.o
	g++ -c FSCAN.cpp -o FSCAN.o
	g++ Start.o Request.o IO.o FIFO.o SSTF.o SCAN.o CSCAN.o FSCAN.o -o Start

clean:
	rm -rf *.o
	rm Start
