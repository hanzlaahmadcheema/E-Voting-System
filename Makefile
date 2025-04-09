# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Folders
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Source subdirectories
CLI_DIR = $(SRC_DIR)/cli
CORE_DIR = $(SRC_DIR)/core
DB_DIR = $(SRC_DIR)/db
MODELS_DIR = $(SRC_DIR)/models

# Object files
OBJS = \
  $(BUILD_DIR)/main.o \
  $(BUILD_DIR)/cli/CLI.o \
  $(BUILD_DIR)/core/VotingSystem.o \
  $(BUILD_DIR)/db/Database.o \
  $(BUILD_DIR)/models/Candidate.o \
  $(BUILD_DIR)/models/Voter.o

# Target executable
TARGET = $(BIN_DIR)/eVotingSystem.exe

# Default target
all: $(TARGET)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/cli/CLI.o: $(CLI_DIR)/CLI.cpp
	@mkdir -p $(BUILD_DIR)/cli
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/core/VotingSystem.o: $(CORE_DIR)/VotingSystem.cpp
	@mkdir -p $(BUILD_DIR)/core
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/db/Database.o: $(DB_DIR)/Database.cpp
	@mkdir -p $(BUILD_DIR)/db
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/models/Candidate.o: $(MODELS_DIR)/Candidate.cpp
	@mkdir -p $(BUILD_DIR)/models
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/models/Voter.o: $(MODELS_DIR)/Voter.cpp
	@mkdir -p $(BUILD_DIR)/models
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: all
	$(TARGET)

clean:
	del /Q $(BUILD_DIR)\*.o $(BUILD_DIR)\cli\*.o $(BUILD_DIR)\core\*.o $(BUILD_DIR)\db\*.o $(BUILD_DIR)\models\*.o 2>nul
	del /Q $(BIN_DIR)\*.exe 2>nul

.PHONY: all clean run
