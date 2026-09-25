PROGRAM := iasm
CXX := g++
CXXFLAGS := -std=c++23

LINK.o := $(CXX)

.PHONY: all clean handin

main: main.cpp
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) main.cpp

all : main run

clean :
	rm -f main.o 

run:
	./${PROGRAM}