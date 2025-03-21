CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Find all source files
SOURCES = $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
EXECUTABLE = $(BIN_DIR)/eVotingSystem

# Ensure the necessary directories exist
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# Ensure object files are compiled properly
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)  # Create the required subdirectory
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run: all
	./$(EXECUTABLE)