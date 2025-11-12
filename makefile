# ==== Configuration ====
# Compiler
CXX := g++

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin

# Executable Name
OUT := snake
TARGET := $(BIN_DIR)/$(OUT)

# ==== Flags ====

# Get flags from pkg-config and add include path for our headers
CXXFLAGS := $(shell pkg-config --cflags sdl2) -I$(INC_DIR) -Wall -Wextra -std=c++17

# Get linker flags from pkg-config
LDFLAGS_DYNAMIC := $(shell pkg-config --libs sdl2)

# ==== Sources & Targets ====

# Find all .cpp files in the src directory
SRC := $(wildcard $(SRC_DIR)/*.cpp)

# Create a list of .o files in the obj directory, e.g., src/main.cpp -> build/obj/main.o
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Find all .h files in the include directory
DEPS := $(wildcard $(INC_DIR)/*.h)

# ==== Default Rule ====

# 'all' now depends on the final executable target
all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS_DYNAMIC)

# ==== Compilation Rule ====

# Generic rule to compile any .cpp from src/ into a .o file in build/obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==== Utility Rules ====

# Rule to run the game
run: $(TARGET)
	./$(TARGET)

# Rule to clean up all build artifacts
clean:
	rm -rf $(BUILD_DIR)

# ==== Phony Targets ====
# Tells make that 'all', 'clean', and 'run' aren't actual files
.PHONY: all clean run