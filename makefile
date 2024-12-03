# # Please implement your Makefile rules and targets below.
# # Customize this file to define how to build your project.
# ARGS = config_file.txt

# # Please implement your Makefile rules and targets below.
# # Customize this file to define how to build your project.
# all: compile

# compile: bin bin/main

# bin:
# 	mkdir bin

# bin/main: bin/Simulation.o bin/main.o
# 	g++ -o bin/main bin/Simulation.o bin/main.o

# bin/Simulation.o: src/Simulation.cpp
# 	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp

# bin/main.o: src/main.cpp
# 	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

# clean: 
# 	rm -f bin/*

# run: bin/main
# 	./bin/main $(ARGS)
####################################################################################
# # Arguments
# ARGS = config_file.txt

# # Compiler and Flags
# CXX = g++
# CXXFLAGS = -Wall -Wextra -Wpedantic -Weffc++ -std=c++11
# INCLUDE = -Iinclude

# # Directories
# SRC_DIR = src
# BIN_DIR = bin
# INCLUDE_DIR = include

# # Source Files
# SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SOURCES))

# # Target Executable
# TARGET = $(BIN_DIR)/main

# # Default Target
# all: $(TARGET)

# # Build Target
# $(TARGET): $(OBJECTS)
# 	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# # Build Object Files
# $(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	@mkdir -p $(BIN_DIR)
# 	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# # Clean Target
# clean:
# 	rm -rf $(BIN_DIR)

# # Run Target
# run: $(TARGET)
# 	./$(TARGET) $(ARGS)

# Arguments
ARGS = config_file.txt

# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -Weffc++ -std=c++11
DEBUGFLAGS = -g
INCLUDE = -Iinclude

# Directories
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include

# Source Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SOURCES))

# Target Executable
TARGET = $(BIN_DIR)/main

# Default Target
all: $(TARGET)

# Build Target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Build Object Files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# Debug Target
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: clean $(TARGET)

# Clean Target
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

# Run Target
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Phony Targets
.PHONY: all clean debug run
