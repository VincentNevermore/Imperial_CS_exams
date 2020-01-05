river: main.o river.o
	g++ -g main.o river.o -o river
main.o: main.cpp river.h
	g++ -Wall -g -c main.cpp
river.o: river.cpp river.h
	g++ -Wall -g -c river.cpp
clean: rm -f *.0 river