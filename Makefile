# Compiler and flags
CXX := g++
CXXFLAGS := -g #-O0 -Wall -Wextra

# Libraries
LDFLAGS := -lSDL2 -lGLEW -lGL

# Source and output directories
SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/opengl

# Source files and object files
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)

# Run the application
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
