# Compiler and flags
CFLAGS	:= -g -Wall -O3
LDLIBS	:=
LDFLAGS	:=
CC		:= gcc

# Target binary
TARGET	:= Assembler

# Directories
SRC_DIR		:= src
BUILD_DIR	:= build
BIN_DIR		:= bin

# Source and object files
SRCS	:= $(wildcard $(SRC_DIR)/*.c)
# Map src/filename.c to build/filename.o
OBJECTS	:= $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
# Executable path
EXEC	:= $(BIN_DIR)/$(TARGET)

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJECTS)

# Link object files into executable
$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@
	@echo "Build successful"

# Compile C source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(BUILD_DIR)/*.o
	rm $(EXEC)
	@echo "Clean complete"
