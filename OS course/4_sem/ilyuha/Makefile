#Makefile for diff

start: changes.o main.o Calculation.o RunProg.o DescentParser.o
	g++ -o start changes.o main.o Calculation.o RunProg.o DescentParser.o

changes.o: changes.cpp changes.hpp
	g++ -c changes.cpp

Calculation.o: Calculation.cpp Calculation.hpp
	g++ -c Calculation.cpp

RunProg.o: RunProg.cpp RunProg.hpp
	g++ -c RunProg.cpp

DescentParser.o: DescentParser.cpp DescentParser.hpp
	g++ -c DescentParser.cpp

main.o: main.cpp RunProg.hpp
	g++ -c main.cpp

clean:
	rm -f *.o
	rm -f start
