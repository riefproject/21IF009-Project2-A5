# Scripts Directory

This directory contains build automation and utility scripts for the Block Shooter game project.

## 📑 Files

### Build Scripts

-   `build.bat` - Windows build script

    -   Compiles source files
    -   Links object files
    -   Creates executable
    -   Handles dependencies

-   `build.sh` - Linux/macOS build script
    -   Same functionality as build.bat
    -   UNIX-compatible

### Cleanup Scripts

-   `clean.bat` - Windows cleanup script

    -   Removes build artifacts
    -   Cleans output directories
    -   Resets build state

-   `clean.sh` - Linux/macOS cleanup script
    -   Same functionality as clean.bat
    -   UNIX-compatible

### Test Scripts

-   `test.bat` - Windows test runner
    -   Runs unit tests
    -   Validates game components
    -   Reports test results

## 📋 Script Details

### Build Process

1. Creates necessary directories
2. Compiles each source file
3. Links object files
4. Generates executable
5. Runs basic validation

### Clean Process

1. Removes bin directory
2. Removes build/output
3. Deletes temporary files

### Test Process

1. Builds test executable
2. Runs unit tests
3. Reports results
4. Cleans test artifacts

## ⚙️ Configuration

Scripts use these environment variables:

-   `CC` - Compiler (defaults to gcc)
-   `CFLAGS` - Compiler flags
-   `LDFLAGS` - Linker flags

## 🔍 Error Handling

-   Checks compilation success
-   Validates directory creation
-   Reports error messages
-   Exits with proper codes

## 📚 Dependencies

-   GCC/MinGW compiler
-   Raylib library
-   Make (optional)
