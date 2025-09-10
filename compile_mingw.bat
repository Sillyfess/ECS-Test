@echo off
echo Compiling OpenGL Cube Renderer with MinGW64...

set GCC=C:\mingw64\bin\gcc.exe
set GPP=C:\mingw64\bin\g++.exe

if not exist "%GCC%" (
    echo Error: GCC not found at %GCC%
    pause
    exit /b 1
)

if not exist build mkdir build

echo Compiling GLAD...
%GCC% -c deps/glad/src/glad.c -I deps/glad/include -o build/glad.o

echo Compiling source files...
%GPP% -c src/Shader.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/Shader.o
%GPP% -c src/Camera.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/Camera.o  
%GPP% -c src/CubeRenderer.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/CubeRenderer.o
%GPP% -c src/main.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/main.o

echo Linking...
%GPP% build/glad.o build/Shader.o build/Camera.o build/CubeRenderer.o build/main.o -o build/CubeRenderer.exe -L deps/glfw/lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32

if errorlevel 1 (
    echo.
    echo Link failed. Trying alternative...
    %GPP% build/glad.o build/Shader.o build/Camera.o build/CubeRenderer.o build/main.o deps/glfw/lib/libglfw3.a -o build/CubeRenderer.exe -lopengl32 -lgdi32 -luser32 -lshell32
)

echo.
if exist build\CubeRenderer.exe (
    echo Build successful! 
    echo Executable is at build\CubeRenderer.exe
    echo.
    echo Copying shaders to build directory...
    xcopy /E /I /Y shaders build\shaders >nul
    echo.
    echo You can now run: build\CubeRenderer.exe
) else (
    echo Build failed. Checking for missing dependencies...
)
pause