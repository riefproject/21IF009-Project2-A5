REM filepath: /d:/Academic/Semester2/21IF009-Project2-A5/scripts/clean.bat
@echo off
echo Cleaning build directories...

:: Remove the bin directory if it exists
if exist bin rmdir /s /q bin

:: Remove the build\output directory if it exists
if exist build\output rmdir /s /q build\output

echo Clean complete!
pause