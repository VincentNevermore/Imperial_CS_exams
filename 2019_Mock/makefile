spell: main.o spell.o
	g++ -g main.o spell.o -o spell
main.o: main.cpp spell.h
	g++ -Wall -g -c main.cpp
spell.o: spell.cpp spell.h
	g++ -Wall -g -c spell.cpp
clean: rm -f *.0 spell