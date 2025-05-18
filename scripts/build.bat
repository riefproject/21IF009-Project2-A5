@echo off
setlocal enabledelayedexpansion

REM Compiler
set CC=gcc

REM Flags
set CFLAGS=-Wall -Wextra -Iinclude -Ivendor/raylib-v5.5/include -Ivendor/reestruct-v0.1.0/include
set LDFLAGS=vendor/raylib-v5.5/lib/libraylib.a -lopengl32 -lgdi32 -lwinmm
set RSTFLAGS=vendor/reestruct-v0.1.0/lib/libreestruct.a
set GRFLAGS=-static-libgcc -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32

REM Paths
set SRC_PATH=src library
set OBJ_PATH=build\output
set BIN_PATH=bin
set TMP_PATH=temp

REM Create necessary directories
if not exist %OBJ_PATH%\src mkdir %OBJ_PATH%\src
if not exist %OBJ_PATH%\library mkdir %OBJ_PATH%\library
if not exist %BIN_PATH% mkdir %BIN_PATH%

echo  Compiling source files...

REM Compile main.c
%CC% %CFLAGS% -c src\main.c -o %OBJ_PATH%\src\main.o
if %errorlevel% neq 0 (
    echo Compilation of src\main.c failed!
    exit /b %errorlevel%
)

REM Compile library files
for %%f in (arief naira raffi faliq goklas) do (
    echo  Compiling library\%%f.c...
    %CC% %CFLAGS% -c library\%%f.c -o %OBJ_PATH%\library\%%f.o
    if %errorlevel% neq 0 (
        echo Compilation of library\%%f.c failed!
        exit /b %errorlevel%
    )
)

echo  Linking...
%CC% %OBJ_PATH%\src\main.o %OBJ_PATH%\library\arief.o %OBJ_PATH%\library\naira.o %OBJ_PATH%\library\raffi.o %OBJ_PATH%\library\faliq.o %OBJ_PATH%\library\goklas.o -o %BIN_PATH%\game.exe %LDFLAGS% %RSTFLAGS%
if %errorlevel% neq 0 (
    echo Linking failed!
    exit /b %errorlevel%
)

cls
echo  Build successful! Run '%BIN_PATH%\game.exe'

REM Run the game
echo  Running game...
%BIN_PATH%\game.exe
if %errorlevel% neq 0 (
    echo Game crashed!
    exit /b %errorlevel%
)