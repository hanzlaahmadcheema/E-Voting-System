# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./include

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable
TARGET = $(BIN_DIR)/eVotingSystem

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: all
	./$(TARGET)
