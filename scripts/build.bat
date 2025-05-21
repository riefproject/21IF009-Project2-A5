@echo off
setlocal enabledelayedexpansion

REM Compiler settings
set CC=gcc
set WINDRES=windres

REM Flags
set WNOFLAGS=-Wno-unused-variable -Wno-switch -Wno-unused-parameter
set CFLAGS=%WNOFLAGS% -Iinclude -Ivendor/raylib-v5.5/include -Ivendor/reestruct-v0.1.0/include
set LDFLAGS=vendor/raylib-v5.5/lib/libraylib.a -lopengl32 -lgdi32 -lwinmm
set RSTFLAGS=vendor/reestruct-v0.1.0/lib/libreestruct.a

REM Paths
set SRC_PATH=src library
set OBJ_PATH=build\output
set BIN_PATH=bin
set TMP_PATH=temp

REM Target
set TARGET_NAME="BlockShooter.exe"
set TARGET=%BIN_PATH%\%TARGET_NAME%
set TEST_TARGET=%BIN_PATH%\test.exe

REM Resource file
set RC_FILE=assets\resource.rc
set RC_OBJ=%OBJ_PATH%\resource.o

REM Check for command-line argument
if "%~1"=="" (
    call :build
    goto :eof
)

if /i "%~1"=="build" (
    call :build
) else if /i "%~1"=="run" (
    call :run
) else if /i "%~1"=="clean" (
    call :clean
) else if /i "%~1"=="rebuild" (
    call :rebuild
) else if /i "%~1"=="test" (
    call :test
) else if /i "%~1"=="clean-test" (
    call :clean_test
) else if /i "%~1"=="help" (
    call :show_help
) else (
    echo Unknown command: %1
    call :show_help
)

goto :eof

:show_help
echo Usage: build.bat [command]
echo.
echo Commands:
echo   No command or 'build' - Build the game
echo   run     - Build and run the game
echo   clean   - Clean build directories
echo   rebuild - Clean and then build and run
echo   test    - Run tests
echo   clean-test - Clean test files
echo   help    - Show this help message
goto :eof

:rebuild
echo [REBUILD] Rebuilding from scratch...
call :clean
if %errorlevel% neq 0 exit /b %errorlevel%
call :run
goto :eof

:build
echo Creating necessary directories...
if not exist %OBJ_PATH%\src mkdir %OBJ_PATH%\src
if not exist %OBJ_PATH%\library mkdir %OBJ_PATH%\library
if not exist %BIN_PATH% mkdir %BIN_PATH%

REM Compile resource file
echo [BUILD] Compiling resources %RC_FILE%...
%WINDRES% %RC_FILE% -o %RC_OBJ%
if %errorlevel% neq 0 (
    echo Compilation of resource file failed!
    exit /b %errorlevel%
)

echo [BUILD] Compiling source files...

REM Compile main.c
%CC% %CFLAGS% -c src\main.c -o %OBJ_PATH%\src\main.o
if %errorlevel% neq 0 (
    echo Compilation of src\main.c failed!
    exit /b %errorlevel%
)

REM Compile library files
for %%f in (arief naira raffi faliq goklas) do (
    echo [BUILD] Compiling library\%%f.c...
    %CC% %CFLAGS% -c library\%%f.c -o %OBJ_PATH%\library\%%f.o
    if %errorlevel% neq 0 (
        echo Compilation of library\%%f.c failed!
        exit /b %errorlevel%
    )
)

echo [LINK] Linking...
%CC% %OBJ_PATH%\src\main.o %OBJ_PATH%\library\arief.o %OBJ_PATH%\library\naira.o %OBJ_PATH%\library\raffi.o %OBJ_PATH%\library\faliq.o %OBJ_PATH%\library\goklas.o %RC_OBJ% -o %TARGET% %LDFLAGS% %RSTFLAGS%
if %errorlevel% neq 0 (
    echo Linking failed!
    exit /b %errorlevel%
)

cls
echo [DONE] Build successful! Run '%TARGET%'
goto :eof

:run
call :build
if %errorlevel% neq 0 exit /b %errorlevel%

echo [RUN] Running game...
%TARGET%
if %errorlevel% neq 0 (
    echo Game crashed with error code %errorlevel%!
    exit /b %errorlevel%
)
goto :eof

:clean
echo [CLEAN] Cleaning build directory...
if exist %OBJ_PATH% rmdir /s /q %OBJ_PATH%
if exist %BIN_PATH% rmdir /s /q %BIN_PATH%
echo [DONE] Clean complete!
goto :eof