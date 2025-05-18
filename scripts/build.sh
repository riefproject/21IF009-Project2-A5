#!/bin/bash

echo "Compiling Block Shooter Game..."

# Ensure necessary directories exist
mkdir -p bin build/output/src build/output/library

# Set compiler flags to match Makefile
CFLAGS="-Wall -Wextra -Iinclude -Ivendor/raylib-v5.5/include -Ivendor/reestruct-v0.1.0/include"
LDFLAGS="vendor/raylib-v5.5/lib/libraylib.a -lopengl32 -lgdi32 -lwinmm"
RSTFLAGS="vendor/reestruct-v0.1.0/lib/libreestruct.a"

# Function to compile source files
compile() {
    local src_file=$1
    local out_file=$2

    echo "🔨 Compiling $src_file..."
    gcc $CFLAGS -c "$src_file" -o "$out_file"

    if [ $? -ne 0 ]; then
        echo "❌ Compilation of $src_file failed!"
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
echo "🔧 Linking..."
gcc build/output/src/main.o \
    build/output/library/arief.o \
    build/output/library/naira.o \
    build/output/library/raffi.o \
    build/output/library/faliq.o \
    build/output/library/goklas.o \
    -o bin/game.exe $LDFLAGS $RSTFLAGS

if [ $? -ne 0 ]; then
    echo "❌ Linking failed!"
    exit 1
fi

# Run the game
echo "🚀 Running game..."
sleep 1
./bin/game.exe || echo "❌ Game failed to start! Check for errors."