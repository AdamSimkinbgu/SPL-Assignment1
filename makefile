# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
ARGS = config_file.txt

# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
all: compile

compile: bin bin/main

bin:
	mkdir bin

bin/main: bin/Simulation.o bin/main.o
	g++ -o bin/main bin/Simulation.o bin/main.o

bin/Simulation.o: src/Simulation.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp

bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

clean: 
	rm -f bin/*

run: bin/main
	./bin/main $(ARGS)