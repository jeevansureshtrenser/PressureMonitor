#*******************************************************************************
# File      	: Makefile
# Description 	: Makefile for Pressure Monitor Application
# Author 		: Jeevan Suresh
# License   	: Copyright (c) 2026 Trenser 
#                    All Rights Reserved
#*******************************************************************************

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -Iinclude

# Target executable
TARGET = build/pressure_monitor

# Source and object files
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

# Default rule
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files inside build/
build/%.o: src/%.c | build
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@
# Create build directory if it doesn't exist
build:
	@mkdir -p build							
# Clean rule to remove build artifacts
clean:
	@echo "Cleaning up..."
	@rm -rf build
	@echo "Done."

#end of file
