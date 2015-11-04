makefile:

CC = g++
CFLAGS  = -g -Wall -std=c++11

all: openscan

openscan: main.o geometry.o cvmethods.o
	$(CC) $(CFLAGS) main.o geometry.o cvmethods.o -o openscan

main.o: main.cpp
	$(CC) $(CFLAGS) -cpp main.cpp

geometry.o: geometry.cpp
	$(CC) $(CFLAGS) -cpp geometry.cpp
	
cvmethods.o: cvmethods.cpp
	$(CC) $(CFLAGS) -cpp cvmethods.cpp
	


