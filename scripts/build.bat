@echo off
setlocal enabledelayedexpansion

REM pake array karna akan diiterasi. Kalo mau nambah subfolder tambahin aja
set "SRC_DIRS=src library"

set "BUILD_DIR=build\output"
set "BIN_DIR=bin"
set "EXE_NAME=BlockShooter.exe"
set "EXE_PATH=%BIN_DIR%\%EXE_NAME%"
set "TEST_EXE_NAME=test.exe"
set "TEST_EXE_PATH=%BIN_DIR%\%TEST_EXE_NAME%"
set "RESOURCE_RC=assets\resource.rc"
set "RESOURCE_RES=%BUILD_DIR%\resource.o"
set "TMP_PATH=temp"

REM bukan array karna ga akan diiterasi (cuma flag). Tambahin aja kalo butuh subfolder tambahan
set "WNO=-Wno-unused-variable -Wno-switch -Wno-unused-parameter"

set "CFLAGS=%WNO% -Iinclude -Ivendor\raylib-v5.5\include -Ivendor\reestruct-v0.1.0\include"
set "LDFLAGS=vendor\raylib-v5.5\lib\libraylib.a -lopengl32 -lgdi32 -lwinmm"
set "RSTFLAGS=vendor\reestruct-v0.1.0\lib\libreestruct.a"
set "OBJECT_FILES="

REM =====================================================================================
REM . . . FUNCTIONS . . .
REM =====================================================================================

goto :main

:clean
echo [CLEAN] Cleaning build directories...
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
if exist "%BIN_DIR%" rmdir /s /q "%BIN_DIR%"
if exist "%RESOURCE_RES%" del /q "%RESOURCE_RES%"
echo [DONE] Clean complete!
goto :eof

:clean_test
echo [CLEAN] Cleaning test files...
if exist "%TEST_EXE_PATH%" del /q "%TEST_EXE_PATH%"
if exist "%BUILD_DIR%\%TMP_PATH%" rmdir /s /q "%BUILD_DIR%\%TMP_PATH%"
echo [DONE] Test clean complete!
goto :eof

:compile_if_needed
set "src_file=%~1"
set "out_file=%~2"

REM Create directory if it doesn't exist
for %%F in ("%out_file%") do (
    if not exist "%%~dpF" mkdir "%%~dpF"
)

REM Check if compilation is needed
set "need_compile=0"
if not exist "%out_file%" set "need_compile=1"
if exist "%out_file%" (
    for %%A in ("%src_file%") do set "src_time=%%~tA"
    for %%A in ("%out_file%") do set "out_time=%%~tA"
    if "!src_time!" GTR "!out_time!" set "need_compile=1"
)

if "!need_compile!"=="1" (
    echo [COMPILE] Compiling %src_file%...
    gcc %CFLAGS% -c "%src_file%" -o "%out_file%"
    if !errorlevel! neq 0 (
        echo [ERROR] Compilation failed: %src_file% 
        exit /b 1
    )
) else (
    echo [DONE] Skipping %src_file% ^(up to date^) 
)

set "OBJECT_FILES=%OBJECT_FILES% "%out_file%""
goto :eof

:compile_sources
for %%d in (%SRC_DIRS%) do (
    for %%f in ("%%d\*.c") do (
        if exist "%%f" (
            set "filename=%%~nf"
            set "out_file=%BUILD_DIR%\%%d\!filename!.o"
            call :compile_if_needed "%%f" "!out_file!"
            if !errorlevel! neq 0 exit /b 1
        )
    )
)
goto :eof

:compile_resource_if_needed
set "need_compile=0"
if not exist "%RESOURCE_RES%" set "need_compile=1"
if exist "%RESOURCE_RES%" (
    for %%A in ("%RESOURCE_RC%") do set "rc_time=%%~tA"
    for %%A in ("%RESOURCE_RES%") do set "res_time=%%~tA"
    if "!rc_time!" GTR "!res_time!" set "need_compile=1"
)

if "!need_compile!"=="1" (
    echo [COMPILING] Compiling resource file...
    for %%F in ("%RESOURCE_RES%") do (
        if not exist "%%~dpF" mkdir "%%~dpF"
    )
    windres "%RESOURCE_RC%" -o "%RESOURCE_RES%"
    if !errorlevel! neq 0 (
        echo [ERROR] Resource compilation failed! 
        exit /b 1
    )
) else (
    echo [DONE] Skipping resource compilation ^(up to date^) 
)
goto :eof

:link_if_needed
call :compile_resource_if_needed
if !errorlevel! neq 0 exit /b 1

set "need_link=0"
if not exist "%EXE_PATH%" set "need_link=1"
if exist "%EXE_PATH%" (
    for %%A in ("%RESOURCE_RES%") do set "res_time=%%~tA"
    for %%A in ("%EXE_PATH%") do set "exe_time=%%~tA"
    if "!res_time!" GTR "!exe_time!" set "need_link=1"
    
    REM Check object files (simplified - would need more complex logic for full implementation)
    if "!need_link!"=="0" (
        for %%f in (%OBJECT_FILES%) do (
            for %%A in ("%%~f") do set "obj_time=%%~tA"
            if "!obj_time!" GTR "!exe_time!" set "need_link=1"
        )
    )
)

if "!need_link!"=="1" (
    echo [LINKING] Linking...
    gcc %OBJECT_FILES% "%RESOURCE_RES%" -o "%EXE_PATH%" %LDFLAGS% %RSTFLAGS%
    if !errorlevel! neq 0 (
        echo [ERROR] Linking failed! 
        exit /b 1
    )
) else (
    echo [DONE] Skipping linking ^(up to date^) 
)
goto :eof

:build
echo [BUILD] Starting build process...
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"
call :compile_sources
if !errorlevel! neq 0 exit /b 1
call :link_if_needed
if !errorlevel! neq 0 exit /b 1

cls
echo [DONE] Build successful! Run './%EXE_PATH%'
goto :eof

:run
call :build
if !errorlevel! neq 0 exit /b 1

echo.
echo [RUN] Running %BLUE%BlockShooter...
"%EXE_PATH%"

cls
if !errorlevel! equ 0 (
    echo.
    echo.
    echo Thanks for playing Block Shooter
)
goto :eof

:test
call :clean_test
if !errorlevel! neq 0 exit /b 1

echo Creating necessary directories...
if not exist "%BUILD_DIR%\%TMP_PATH%" mkdir "%BUILD_DIR%\%TMP_PATH%"
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"

echo [COMPILE] Compiling test file %TMP_PATH%\main.c...
gcc %CFLAGS% -c "%TMP_PATH%\main.c" -o "%BUILD_DIR%\%TMP_PATH%\main.o"
if !errorlevel! neq 0 (
    echo [ERROR] Compilation of %TMP_PATH%\main.c failed! 
    exit /b 1
)

REM Reset OBJECT_FILES for test compilation
set "OBJECT_FILES="

REM Compile library files for tests
for %%f in ("library\*.c") do (
    if exist "%%f" (
        set "filename=%%~nf"
        set "out_file=%BUILD_DIR%\library\!filename!.o"
        call :compile_if_needed "%%f" "!out_file!"
        if !errorlevel! neq 0 exit /b 1
    )
)

echo [LINKING] Linking test executable...
gcc "%BUILD_DIR%\%TMP_PATH%\main.o" %OBJECT_FILES% -o "%TEST_EXE_PATH%" %LDFLAGS%

if !errorlevel! neq 0 (
    echo [ERROR] Test linking failed! 
    exit /b 1
)

echo [DONE] Test build successful!
echo [TEST] Running tests...
"%TEST_EXE_PATH%"

if !errorlevel! neq 0 (
    echo [ERROR] Tests failed with error code !errorlevel!! 
    exit /b 1
)
goto :eof

:rebuild
echo [REBUILD] Rebuilding from scratch...
call :clean
if !errorlevel! neq 0 exit /b 1
call :run
goto :eof

:show_help
echo.
echo %BOLD%%BLUE%BlockShooter%BOLD% Build Script
echo ============================================================
echo.
echo Usage: %~nx0 [--debug] [clean^|build^|run^|rebuild^|test^|clean-test] [--help]
echo.
echo Commands:
echo   build       : Compile dan link ^(incremental^)
echo   run         : Build dan jalankan game
echo   clean       : Bersihkan hasil build
echo   rebuild     : Bersihkan lalu build dan run
echo   test        : Jalankan tests
echo   clean-test  : Bersihkan file test
echo   --debug     : Aktifkan mode debug
echo   --help      : Tampilkan pesan ini
echo.
echo Notes:
echo   --help harus dipanggil sendiri, tanpa param lain.
goto :eof

:main
REM Parse arguments
set "DEBUG_FLAG=0"
set "HELP_FLAG=0"
set "COMMAND="
set "EXTRA_ARGS=0"

:parse_loop
if "%~1"=="" goto :parse_done
if "%~1"=="--debug" (
    set "DEBUG_FLAG=1"
    set "EXTRA_ARGS=1"
) else if "%~1"=="--help" (
    set "HELP_FLAG=1"
    set "EXTRA_ARGS=1"
) else (
    if "!COMMAND!"=="" (
        set "COMMAND=%~1"
    ) else (
        set "EXTRA_ARGS=1"
    )
)
shift
goto :parse_loop

:parse_done
if "!HELP_FLAG!"=="1" (
    if "!EXTRA_ARGS!"=="1" (
        echo [ERROR] Error: --help harus dipanggil tanpa parameter lain.
        call :show_help
        exit /b 1
    )
    call :show_help
    exit /b 0
)

if "!DEBUG_FLAG!"=="1" (
    echo Debug mode enabled
)

REM Execute command
if "!COMMAND!"=="" set "COMMAND=build"

if "!COMMAND!"=="clean" (
    call :clean
) else if "!COMMAND!"=="rebuild" (
    call :rebuild
) else if "!COMMAND!"=="run" (
    call :run
) else if "!COMMAND!"=="test" (
    call :test
) else if "!COMMAND!"=="clean-test" (
    call :clean_test
) else if "!COMMAND!"=="build" (
    call :build
    call :run
) else (
    echo [ERROR] Unknown parameter: !COMMAND! 
    call :show_help
    exit /b 1
)

exit /b 0