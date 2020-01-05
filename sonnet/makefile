sonnet: main.o sonnet.o
	g++ -g main.o sonnet.o -o sonnet
main.o: main.cpp sonnet.h
	g++ -Wall -g -c main.cpp
sonnet.o: sonnet.cpp sonnet.h
	g++ -Wall -g -c sonnet.cpp
clean: rm -f *.0 sonnet