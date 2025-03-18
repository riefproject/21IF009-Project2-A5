#!/bin/bash

echo "Compiling Block Shooter Game..."

# Ensure necessary directories exist
mkdir -p bin build/output/src build/output/library

# Function to compile source files
compile() {
    local src_file=$1
    local out_file=$2

    echo "Compiling $src_file..."
    gcc -Wall -Wextra -Iinclude -c "$src_file" -o "$out_file"

    if [ $? -ne 0 ]; then
        echo "Compilation of $src_file failed!"
        exit 1
    fi
}

# Compile all source files
compile "src/main.c" "build/output/src/main.o"
compile "library/arief.c" "build/output/library/arief.o"
compile "library/naira.c" "build/output/library/naira.o"
compile "library/raffi.c" "build/output/library/raffi.o"
compile "library/faliq.c" "build/output/library/faliq.o"
compile "library/goklas.c" "build/output/library/goklas.o"

# Linking all object files into the final executable
echo "Linking object files..."
gcc build/output/src/main.o \
    build/output/library/arief.o \
    build/output/library/naira.o \
    build/output/library/raffi.o \
    build/output/library/faliq.o \
    build/output/library/goklas.o \
    -o bin/game -lm -lraylib

if [ $? -ne 0 ]; then
    echo "Linking failed!"
    exit 1
fi

# Run the game (Fix untuk Git Bash & WSL)
echo "Running game..."
sleep 1
exec ./bin/game || echo "Game failed to start! Check for errors."
