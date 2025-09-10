@echo off
echo Compiling OpenGL ECS Cube Renderer with MinGW64...

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

echo Compiling ECS Component...
%GPP% -std=c++17 -c src/ECS/Component.cpp -I include -I deps/glm -o build/Component.o

echo Compiling source files...
%GPP% -std=c++17 -c src/Shader.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/Shader.o
%GPP% -std=c++17 -c src/Camera.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/Camera.o  
%GPP% -std=c++17 -c src/CubeRenderer.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/CubeRenderer.o
%GPP% -std=c++17 -c src/main_ecs.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/main_ecs.o

echo Linking...
%GPP% build/glad.o build/Component.o build/Shader.o build/Camera.o build/CubeRenderer.o build/main_ecs.o -o build/CubeRendererECS.exe -L deps/glfw/lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32

if errorlevel 1 (
    echo.
    echo Link failed. Trying alternative...
    %GPP% build/glad.o build/Component.o build/Shader.o build/Camera.o build/CubeRenderer.o build/main_ecs.o deps/glfw/lib/libglfw3.a -o build/CubeRendererECS.exe -lopengl32 -lgdi32 -luser32 -lshell32
)

echo.
if exist build\CubeRendererECS.exe (
    echo Build successful! 
    echo Executable is at build\CubeRendererECS.exe
    echo.
    echo Copying shaders to build directory...
    xcopy /E /I /Y shaders build\shaders >nul
    echo.
    echo You can now run: build\CubeRendererECS.exe
) else (
    echo Build failed. Checking for missing dependencies...
    dir build\*.o
)
pause