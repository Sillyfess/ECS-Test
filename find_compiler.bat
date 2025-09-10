@echo off
echo Searching for C++ compilers...
echo.

where gcc.exe 2>nul
if %errorlevel%==0 (
    echo Found GCC at:
    where gcc.exe
    gcc --version
)

where g++.exe 2>nul  
if %errorlevel%==0 (
    echo Found G++ at:
    where g++.exe
)

where cl.exe 2>nul
if %errorlevel%==0 (
    echo Found MSVC at:
    where cl.exe
)

where clang.exe 2>nul
if %errorlevel%==0 (
    echo Found Clang at:
    where clang.exe
)

where clang++.exe 2>nul
if %errorlevel%==0 (
    echo Found Clang++ at:
    where clang++.exe
)

echo.
echo Checking common MinGW locations...
if exist "C:\MinGW\bin\gcc.exe" echo Found: C:\MinGW\bin\gcc.exe
if exist "C:\mingw64\bin\gcc.exe" echo Found: C:\mingw64\bin\gcc.exe  
if exist "C:\msys64\mingw64\bin\gcc.exe" echo Found: C:\msys64\mingw64\bin\gcc.exe
if exist "C:\msys2\mingw64\bin\gcc.exe" echo Found: C:\msys2\mingw64\bin\gcc.exe
if exist "C:\tools\mingw64\bin\gcc.exe" echo Found: C:\tools\mingw64\bin\gcc.exe

echo.
if %errorlevel% neq 0 (
    echo No C++ compiler found in PATH.
    echo Please install MinGW-w64, MSVC, or Clang.
)