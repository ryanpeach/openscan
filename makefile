makefile:

CC = g++
CFLAGS  = -g -Wall -std=c++11

all: openscan

openscan: main.o capture.o cvmethods.o focus.o geometry.o support.o
	$(CC) $(CFLAGS) main.o capture.o cvmethods.o focus.o geometry.o support.o `pkg-config opencv --cflags --libs` -o openscan

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

capture.o: capture.cpp capture.hpp
	$(CC) $(CFLAGS) -c capture.cpp

cvmethods.o: cvmethods.cpp cvmethods.hpp
	$(CC) $(CFLAGS) -c cvmethods.cpp

focus.o: focus.cpp focus.hpp
	$(CC) $(CFLAGS) -c focus.cpp

geometry.o: geometry.cpp geometry.hpp
	$(CC) $(CFLAGS) -c geometry.cpp

support.o: support.cpp support.hpp
	$(CC) $(CFLAGS) -c support.cpp

clear: 
	rm *.o
