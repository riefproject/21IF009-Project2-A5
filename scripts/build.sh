#!/bin/bash

# Compiler settings
CC="gcc"
WINDRES="windres"

# Flags
WNOFLAGS="-Wno-unused-variable -Wno-switch -Wno-unused-parameter"
CFLAGS="$WNOFLAGS -Iinclude -Ivendor/raylib-v5.5/include -Ivendor/reestruct-v0.1.0/include"
LDFLAGS="vendor/raylib-v5.5/lib/libraylib.a -lopengl32 -lgdi32 -lwinmm"
RSTFLAGS="vendor/reestruct-v0.1.0/lib/libreestruct.a"

# Paths
SRC_PATH="src library"
OBJ_PATH="build/output"
BIN_PATH="bin"
TMP_PATH="temp"

# Target
TARGET_NAME="BlockShooter.exe"
TARGET="$BIN_PATH/$TARGET_NAME"
TEST_TARGET="$BIN_PATH/test.exe"

# Resource file
RC_FILE="assets/resource.rc"
RC_OBJ="$OBJ_PATH/resource.o"

# Show help function
show_help() {
    echo "Usage: ./build.sh [command]"
    echo ""
    echo "Commands:"
    echo "  No command or 'build' - Build the game"
    echo "  run     - Build and run the game"
    echo "  clean   - Clean build directories"
    echo "  rebuild - Clean and then build and run"
    echo "  test    - Run tests"
    echo "  clean-test - Clean test files"
    echo "  help    - Show this help message"
}

# Build function
build() {
    echo "Creating necessary directories..."
    mkdir -p "$OBJ_PATH/src" "$OBJ_PATH/library" "$BIN_PATH"
    
    # Compile resource file
    echo "🔨 Compiling resources $RC_FILE..."
    mkdir -p "$(dirname "$RC_OBJ")"
    $WINDRES "$RC_FILE" -o "$RC_OBJ"
    if [ $? -ne 0 ]; then
        echo "❌ Compilation of resource file failed!"
        exit 1
    fi
    
    echo "🔨 Compiling source files..."
    
    # Compile main.c
    echo "🔨 Compiling src/main.c..."
    $CC $CFLAGS -c src/main.c -o "$OBJ_PATH/src/main.o"
    if [ $? -ne 0 ]; then
        echo "❌ Compilation of src/main.c failed!"
        exit 1
    fi
    
    # Compile library files
    for file in arief naira raffi faliq goklas; do
        echo "🔨 Compiling library/$file.c..."
        $CC $CFLAGS -c "library/$file.c" -o "$OBJ_PATH/library/$file.o"
        if [ $? -ne 0 ]; then
            echo "❌ Compilation of library/$file.c failed!"
            exit 1
        fi
    done
    
    echo "🔧 Linking..."
    $CC "$OBJ_PATH/src/main.o" \
        "$OBJ_PATH/library/arief.o" \
        "$OBJ_PATH/library/naira.o" \
        "$OBJ_PATH/library/raffi.o" \
        "$OBJ_PATH/library/faliq.o" \
        "$OBJ_PATH/library/goklas.o" \
        "$RC_OBJ" \
        -o "$TARGET" $LDFLAGS $RSTFLAGS
    
    if [ $? -ne 0 ]; then
        echo "❌ Linking failed!"
        exit 1
    fi
    
    clear
    echo "✅ Build successful! Run './$TARGET'"
}

# Run function
run() {
    build
    if [ $? -ne 0 ]; then
        exit 1
    fi
    
    echo "🚀 Running game..."
    "./$TARGET"
    
    if [ $? -ne 0 ]; then
        echo "❌ Game crashed with error code $?!"
        exit 1
    fi
}

# Clean function
clean() {
    echo "🗑 Cleaning build directory..."
    rm -rf "$OBJ_PATH" "$BIN_PATH"
    echo "✅ Clean complete!"
}

# Rebuild function - clean and then run
rebuild() {
    echo "♻️ Rebuilding from scratch..."
    clean
    if [ $? -ne 0 ]; then
        exit 1
    fi
    run
}

# Test functions
test() {
    clean_test
    if [ $? -ne 0 ]; then
        exit 1
    fi
    
    echo "Creating necessary directories..."
    mkdir -p "$OBJ_PATH/$TMP_PATH" "$BIN_PATH"
    
    echo "🔨 Compiling test file $TMP_PATH/main.c..."
    $CC $CFLAGS -c "$TMP_PATH/main.c" -o "$OBJ_PATH/$TMP_PATH/main.o"
    if [ $? -ne 0 ]; then
        echo "❌ Compilation of $TMP_PATH/main.c failed!"
        exit 1
    fi
    
    # Compile library files for tests
    for file in arief naira raffi faliq goklas; do
        echo "🔨 Compiling library/$file.c for test..."
        $CC $CFLAGS -c "library/$file.c" -o "$OBJ_PATH/library/$file.o"
        if [ $? -ne 0 ]; then
            echo "❌ Compilation of library/$file.c failed!"
            exit 1
        fi
    done
    
    echo "🔧 Linking test executable..."
    $CC "$OBJ_PATH/$TMP_PATH/main.o" \
        "$OBJ_PATH/library/arief.o" \
        "$OBJ_PATH/library/naira.o" \
        "$OBJ_PATH/library/raffi.o" \
        "$OBJ_PATH/library/faliq.o" \
        "$OBJ_PATH/library/goklas.o" \
        -o "$TEST_TARGET" $LDFLAGS
    
    if [ $? -ne 0 ]; then
        echo "❌ Test linking failed!"
        exit 1
    fi
    
    echo "✅ Test build successful!"
    echo "🧪 Running tests..."
    "./$TEST_TARGET"
    
    if [ $? -ne 0 ]; then
        echo "❌ Tests failed with error code $?!"
        exit 1
    fi
}

# Clean test function
clean_test() {
    echo "🗑 Cleaning test files..."
    rm -f "$TEST_TARGET"
    rm -rf "$OBJ_PATH/$TMP_PATH"
    echo "✅ Test clean complete!"
}

# Process arguments
case "$1" in
    "" | "build")
        build
        ;;
    "run")
        run
        ;;
    "clean")
        clean
        ;;
    "rebuild")
        rebuild
        ;;
    "test")
        test
        ;;
    "clean-test")
        clean_test
        ;;
    "help")
        show_help
        ;;
    *)
        echo "❌ Unknown command: $1"
        show_help
        exit 1
        ;;
esac

exit 0