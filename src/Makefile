KEY_FOR_MEMORY=--leak-check=full
KEY_FOR_CREATOR_COND=-DINIT_CONDITIONAL

main: main.cpp creator_cond.o
	g++ -o gpss main.cpp creator_cond.o

creator_cond.o: creator_cond.cpp creator_cond.h
	g++ ${KEY_FOR_CREATOR_COND} -c creator_cond.cpp

memory: main
	valgrind ${KEY_FOR_MEMORY} ./gpss

clear:
	rm -f gpss logger creator_cond.o