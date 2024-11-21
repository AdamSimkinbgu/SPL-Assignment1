# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
ARGS = config_file.txt

all: compile

compile: bin/main.o 

bin/main.o: src/main.cpp bin/Simulation.o
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

bin/Simulation.o: src/Simulation.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp

clean: 
	rm -f bin/*

run: compile
	./bin/main.o $(ARGS)


