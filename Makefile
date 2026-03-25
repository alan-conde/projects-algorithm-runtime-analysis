# ----------------------------
# Project directories
# ----------------------------
SRC_DIR := src
BUILD_DIR := build
SCRIPT_DIR := scripts
OUTPUT_DIR := output

# ----------------------------
# Compiler & flags
# ----------------------------
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra

# ----------------------------
# Source files
# ----------------------------
OBJS := $(BUILD_DIR)/algorithm_timer.o $(BUILD_DIR)/algorithms.o

# ----------------------------
# Target executable
# ----------------------------
TARGET := $(BUILD_DIR)/run_tests

# ----------------------------
# Default rule
# ----------------------------
all: $(TARGET)

# ----------------------------
# Build target executable
# ----------------------------
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# ----------------------------
# Compile each source file into object file
# ----------------------------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ----------------------------
# Ensure build directory exists
# ----------------------------
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ----------------------------
# Clean build artifacts
# ----------------------------
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
	rm -rf $(OUTPUT_DIR)/*

# ----------------------------
# Run the program
# ----------------------------
run: $(TARGET)
	$(TARGET)