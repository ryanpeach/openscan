makefile:

CC = g++
CFLAGS  = -g -Wall -std=c++11

all: openscan

openscan: main.o geometry.o cvmethods.o
	$(CC) $(CFLAGS) -o openscan main.o geometry.o cvmethods.o

geometry.o: geometry.cpp geometry.hpp
	$(CC) $(CFLAGS) -cpp geometry.cpp
	
cvmethods.o: cvmethods.cpp cvmethods.hpp
	$(CC) $(CFLAGS) -cpp cvmethods.cpp
	
main.o: main.cpp
	$(CC) $(CFLAGS) -cpp main.cpp

