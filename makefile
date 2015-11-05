makefile:

CC = g++
CFLAGS  = -g -Wall -std=c++11

all: openscan

openscan: support.o geometry.o focus.o cvmethods.o main.o
	$(CC) $(CFLAGS) main.cpp cvmethods.o focus.o geometry.o support.o -o openscan

cvmethods.o: cvmethods.cpp cvmethods.hpp
	$(CC) $(CFLAGS) -c cvmethods.cpp

focus.o: focus.cpp focus.hpp
	$(CC) $(CFLAGS) -c focus.cpp

geometry.o: geometry.cpp geometry.hpp
	$(CC) $(CFLAGS) -c geometry.cpp

support.o: support.cpp support.hpp
	$(CC) $(CFLAGS) -c support.cpp