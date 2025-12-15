# Scripts Directory

This directory contains build automation and utility scripts for the Block Shooter game project.

## 📑 Files

Build Scripts

-   `build.bat` - Windows build script
    -   Provides command-line interface for Windows users
    -   Uses Windows-specific commands (mkdir, rmdir, etc.)
    -   Designed for CMD/PowerShell environments
-   `build.sh` - Unix/Linux/macOS build script
    -   POSIX shell compatible version
    -   Provides identical functionality to the Windows version
    -   Uses bash syntax and Unix commands
    -   On macOS, auto-detects Homebrew Raylib if `pkg-config` is unavailable

> The Bash script auto-detects the host OS: it uses the bundled Windows Raylib binaries on Windows, and prefers system-installed Raylib via `pkg-config` on Linux/macOS.

## 📄 Usage Examples

-   Windows (CMD/PowerShell)

    ```bash
    build.bat         # Default build
    build.bat run     # Build and run
    build.bat clean   # Clean build artifacts
    build.bat rebuild # Clean, rebuild, and run
    build.bat test    # Run tests
    ```

-   Unix/Linux/macOS (Bash)
    ```bash
    build.sh         # Default build
    build.sh run     # Build and run
    build.sh clean   # Clean build artifacts
    build.sh rebuild # Clean, rebuild, and run
    build.sh test    # Run tests
    ```

## 📋 Script Details

Available Commands
Both scripts support these commands:

-   `build` (default) - Compiles and links the game
-   `rebuild` - Cleans previous build, then builds and runs the game
-   `run` - Builds the game and runs the executable
-   `clean` - Removes build artifacts and directories
-   `test` - Compiles and runs unit tests
-   `clean-test` - Removes test-specific artifacts
-   `help` - Shows usage information

### Build Process

1. Creates necessary directories (`src`, `library`, `bin`)
2. Compiles Windows resources using `windres` (icons, version info)
3. Compiles main source file (`main.c`)
4. Compiles library files:

    - `arief.c`
    - `naira.c`
    - `raffi.c`
    - `faliq.c`
    - `goklas.c`

5. Links all object files with libraries
6. Produces final executable (BlockShooter / BlockShooter.exe)

### Rebuild Process

1. Executes the clean process (removes all build artifacts)
2. Executes the build process (compiles all source files)
3. Runs the executable automatically

### Clean Process

1. Removes `output` directory containing all object files
2. Removes `bin` directory containing executable files
3. Displays confirmation message when complete

### Test Process

1. Cleans previous test artifacts
2. Creates test-specific directories
3. Compiles test main entry point (`temp/main.c`)
4. Compiles library files for testing
5. Links test executable (`bin/test` or `bin/test.exe`)
6. Runs tests and reports results
7. Returns test success/failure status code

## ⚙️ Configuration

Scripts use these environment variables and settings:

-   `CC` - C compiler (defaults to gcc)
-   `WINDRES` - Windows resource compiler
-   `CFLAGS` - Compiler flags including:
    -   Warning suppressions (`-Wno-unused-variable`, `-Wno-switch`, `-Wno-unused-parameter`)
    -   Include paths for header files (`-Iinclude`, `-Ivendor/raylib-v5.5/include`, `-Ivendor/reestruct-v1.0.0/include`)
-   `LDFLAGS` - Linker flags with Raylib `library`
-   `RSTFLAGS` - Reestruct `library` flags

## 🔍 Error Handling

-   Checks compilation status after each file
-   Reports specific file failures with descriptive messages
-   Validates resource compilation
-   Exits with appropriate error codes on failure
-   Aborts subsequent steps if prior steps fail

## 📚 Dependencies

-   **GCC/MinGW compiler** - For C code compilation
-   **Windres** - For Windows resource compilation
-   **Raylib v5.5** - Graphics `library` (expected in raylib-v5.5)
-   **Reestruct v1.0.0** - Data structure `library` (expected in reestruct-v1.0.0)
-   **Raylib on Linux/macOS** - Install via your package manager so `pkg-config --libs raylib` works (the bundled Raylib binaries target Windows)
    -   macOS example: `brew install raylib`
-   **Bash shell** - For Unix environments (build.sh)
-   **Windows Command Prompt** - For Windows environments (build.bat)

## 🖥️ Output Files

-   **Main executable** - `bin/BlockShooter` (Windows: `.exe`)
-   **Test executable** - `bin/test` (Windows: `.exe`)
-   **Object files** - Stored in `output` with directory structure matching source
-   **Resource object** - `resource.o`
