@echo off

echo Configuring..
cmake -H. -B junk/unigine_task_release_windows -G "Visual Studio 14 2015 Win64"
if %ERRORLEVEL% GEQ 1 EXIT /B 1

echo Build Release
cmake --build junk/unigine_task_release_windows --config Release
if %ERRORLEVEL% GEQ 1 EXIT /B 1
