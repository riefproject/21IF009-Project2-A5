@echo off
echo Compiling Brick Shooter Game...

:: Ensure necessary directories exist
if not exist bin mkdir bin
if not exist build\output mkdir build\output
if not exist build\output\src mkdir build\output\src
if not exist build\output\library mkdir build\output\library

:: Set compiler flags to match Makefile
set CFLAGS=-Wall -Wextra -Iinclude -Ivendor/raylib-v5.5/include -Ivendor/reestruct-v0.1.0/include
set LDFLAGS=vendor/raylib-v5.5/lib/libraylib.a -lopengl32 -lgdi32 -lwinmm
set RSTFLAGS=vendor/reestruct-v0.1.0/lib/libreestruct.a

:: Compile source files
echo Compiling source files...
gcc %CFLAGS% -c src\main.c -o build\output\src\main.o
if %errorlevel% neq 0 (
    echo Compilation of src\main.c failed!
    exit /b %errorlevel%
)

:: Compile library files
for %%f in (arief naira raffi faliq goklas) do (
    echo Compiling library\%%f.c...
    gcc %CFLAGS% -c library\%%f.c -o build\output\library\%%f.o
    if %errorlevel% neq 0 (
        echo Compilation of library\%%f.c failed!
        exit /b %errorlevel%
    )
)

:: Link all object files
echo Linking object files...
gcc build\output\src\main.o build\output\library\arief.o build\output\library\naira.o build\output\library\raffi.o build\output\library\faliq.o build\output\library\goklas.o -o bin/game.exe %LDFLAGS% %RSTFLAGS%
if %errorlevel% neq 0 (
    echo Linking failed!
    exit /b %errorlevel%
)

:: Run the game
echo Running game...
bin\game.exe
if %errorlevel% neq 0 (
    echo Game crashed!
    exit /b %errorlevel%
)