#Makefile

CXX=g++
CXXFLAGS=-std=c++1y -Wall -pedantic -g -o
PROG=mem_sim
SOURCES=main.cc
HEADERS=process.hpp frame.hpp framelist.hpp memorymanager.hpp

all: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(PROG) $(SOURCES)
	./$(PROG)
clean:
	rm -f $(PROG)
