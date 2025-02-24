REM 
@echo off
echo Compiling Brick Shooter Game...

:: Ensure necessary directories exist
if not exist bin mkdir bin
if not exist build\output mkdir build\output
if not exist build\output\src mkdir build\output\src
if not exist build\output\library mkdir build\output\library

:: Compile src/main.c into build\output\src\main.o
echo Compiling src\main.c...
gcc -Wall -Wextra -Iinclude -c src\main.c -o build\output\src\main.o
if %errorlevel% neq 0 (
    echo Compilation of src\main.c failed!
    exit /b %errorlevel%
)

:: Compile library\arief.c into build\output\library\arief.o
echo Compiling library\arief.c...
gcc -Wall -Wextra -Iinclude -c library\arief.c -o build\output\library\arief.o
if %errorlevel% neq 0 (
    echo Compilation of library\arief.c failed!
    exit /b %errorlevel%
)

:: Compile library\naira.c into build\output\library\naira.o
echo Compiling library\naira.c...
gcc -Wall -Wextra -Iinclude -c library\naira.c -o build\output\library\naira.o
if %errorlevel% neq 0 (
    echo Compilation of library\naira.c failed!
    exit /b %errorlevel%
)

:: Compile library\raffi.c into build\output\library\raffi.o
echo Compiling library\raffi.c...
gcc -Wall -Wextra -Iinclude -c library\raffi.c -o build\output\library\raffi.o
if %errorlevel% neq 0 (
    echo Compilation of library\raffi.c failed!
    exit /b %errorlevel%
)

:: Compile library\faliq.c into build\output\library\faliq.o
echo Compiling library\faliq.c...
gcc -Wall -Wextra -Iinclude -c library\faliq.c -o build\output\library\faliq.o
if %errorlevel% neq 0 (
    echo Compilation of library\faliq.c failed!
    exit /b %errorlevel%
)

:: Compile library\goklas.c into build\output\library\goklas.o
echo Compiling library\goklas.c...
gcc -Wall -Wextra -Iinclude -c library\goklas.c -o build\output\library\goklas.o
if %errorlevel% neq 0 (
    echo Compilation of library\goklas.c failed!
    exit /b %errorlevel%
)

:: Link all object files into the final executable in bin
echo Linking object files...
gcc build\output\src\main.o build\output\library\arief.o build\output\library\naira.o build\output\library\raffi.o build\output\library\faliq.o build\output\library\goklas.o -o bin/game.exe -lm -lraylib -lopengl32 -lgdi32 -lwinmm
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