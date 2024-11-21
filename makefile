# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
all: compile

compile: bin/Simulation.o

bin/Simulation.o: src/Simulation.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp

clean: 
	rm -f bin/*
