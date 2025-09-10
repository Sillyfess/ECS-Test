@echo off
echo Building OpenGL Cube Renderer...
echo ================================

set PROJECT_DIR=%~dp0..
set GCC=C:\mingw64\bin\gcc.exe
set GPP=C:\mingw64\bin\g++.exe

if not exist "%GCC%" (
    echo Error: GCC not found at %GCC%
    echo Please install MinGW64 or update the path in this script
    pause
    exit /b 1
)

cd /d "%PROJECT_DIR%"

if not exist build mkdir build

echo [1/5] Compiling GLAD...
%GCC% -c deps/glad/src/glad.c -I deps/glad/include -o build/glad.o

echo [2/5] Compiling source files...
%GPP% -c src/Shader.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/Shader.o
%GPP% -c src/Camera.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/Camera.o  
%GPP% -c src/CubeRenderer.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/CubeRenderer.o
%GPP% -c src/main.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/main.o

echo [3/5] Linking executable...
%GPP% build/glad.o build/Shader.o build/Camera.o build/CubeRenderer.o build/main.o -o build/CubeRenderer.exe -L deps/glfw/lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32

if not exist build\CubeRenderer.exe (
    echo.
    echo Link failed. Trying direct library link...
    %GPP% build/glad.o build/Shader.o build/Camera.o build/CubeRenderer.o build/main.o deps/glfw/lib/libglfw3.a -o build/CubeRenderer.exe -lopengl32 -lgdi32 -luser32 -lshell32
)

echo [4/5] Copying shaders...
if not exist build\shaders mkdir build\shaders
xcopy /E /I /Y shaders build\shaders >nul 2>&1

echo [5/5] Build complete!
echo.

if exist build\CubeRenderer.exe (
    echo SUCCESS: Executable created at build\CubeRenderer.exe
    echo.
    echo To run the application: 
    echo   cd build
    echo   CubeRenderer.exe
) else (
    echo ERROR: Build failed. Check the error messages above.
)

pause