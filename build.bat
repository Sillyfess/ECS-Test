@echo off
echo Building OpenGL Cube Renderer...

if not exist build mkdir build
cd build

cmake .. -G "MinGW Makefiles"
if errorlevel 1 (
    echo CMake configuration failed. Trying with Visual Studio...
    cmake .. -G "Visual Studio 17 2022"
    if errorlevel 1 (
        cmake .. -G "Visual Studio 16 2019"
    )
)

cmake --build . --config Release

echo.
echo Build complete! Executable is in build/Release/ or build/
cd ..
pause