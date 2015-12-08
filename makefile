makefile:

CC = g++
CFLAGS  = -g -Wall -std=c++11
LOGFLAGS = -L/usr/lib -llog4cpp -pthread 

all: openscan

openscan: main.o capture.o cvmethods.o focus.o geometry.o support.o filters.o tests.o logger.o
	$(CC) $(CFLAGS) main.o capture.o cvmethods.o focus.o geometry.o support.o filters.o tests.o logger.o `pkg-config opencv --cflags --libs` -o openscan.run $(LOGFLAGS) 

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp $(LOGFLAGS) 

capture.o: capture.cpp capture.hpp
	$(CC) $(CFLAGS) -c capture.cpp $(LOGFLAGS) 

cvmethods.o: cvmethods.cpp cvmethods.hpp
	$(CC) $(CFLAGS) -c cvmethods.cpp $(LOGFLAGS) 

focus.o: focus.cpp focus.hpp
	$(CC) $(CFLAGS) -c focus.cpp $(LOGFLAGS) 


geometry.o: geometry.cpp geometry.hpp
	$(CC) $(CFLAGS) -c geometry.cpp $(LOGFLAGS)

support.o: support.cpp support.hpp
	$(CC) $(CFLAGS) -c support.cpp $(LOGFLAGS)

filters.o: filters.cpp filters.hpp
	$(CC) $(CFLAGS) -c filters.cpp $(LOGFLAGS)

tests.o: tests.cpp tests.hpp
	$(CC) $(CFLAGS) -c tests.cpp $(LOGFLAGS)

logger.o: logger.cpp logger.hpp
	$(CC) $(CFLAGS) -c logger.cpp $(LOGFLAGS)

clear: 
	rm *.o
