# Include Directory

This directory contains all header files (`.h`) used in the Block Shooter game project.

## 📑 Files

-   `all.h` - Main header file that includes all other headers
-   `defines.h` - Core definitions, constants, and data structures
-   `arief.h` - Core game loop and UI management declarations
-   `naira.h` - Power-up system interface declarations
-   `raffi.h` - Score system and persistence declarations
-   `faliq.h` - Player controls and mechanics declarations
-   `goklas.h` - Projectile and collision system declarations

## 🔍 Structure Details

### all.h

-   Centralizes all header includes
-   Prevents circular dependencies
-   Provides single include point for game modules

### defines.h

-   Game constants and macros
-   Core data structures
-   Screen settings
-   Input controls
-   Helper macros

### Module Headers

Each module header contains:

-   Function prototypes
-   Related constants
-   Module-specific structures
-   Type definitions

## 📋 Usage Guidelines

1. Always include `all.h` in source files
2. Use proper include guards
3. Keep declarations and definitions separate
4. Document public interfaces
5. Minimize header dependencies
