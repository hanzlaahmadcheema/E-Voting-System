# FTXUI paths
FTXUI_DIR = ../FTXUI
FTXUI_INC = -I$(FTXUI_DIR)/include
FTXUI_LIB = \
	$(FTXUI_DIR)/build/ftxui-component.lib \
	$(FTXUI_DIR)/build/ftxui-dom.lib \
	$(FTXUI_DIR)/build/ftxui-screen.lib \
	-pthread


# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./include $(FTXUI_INC)

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
	$(CXX) $(CXXFLAGS) -o $@ $^ $(FTXUI_LIB)

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
