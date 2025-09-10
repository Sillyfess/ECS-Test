@echo off
echo Compiling OpenGL Cube Renderer with g++...

if not exist build mkdir build

echo Compiling GLAD...
g++ -c deps/glad/src/glad.c -I deps/glad/include -o build/glad.o

echo Compiling source files...
g++ -c src/Shader.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/Shader.o
g++ -c src/Camera.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/Camera.o  
g++ -c src/CubeRenderer.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/CubeRenderer.o
g++ -c src/main.cpp -I include -I deps/glad/include -I deps/glm -I deps/glfw/include -o build/main.o

echo Linking...
g++ build/glad.o build/Shader.o build/Camera.o build/CubeRenderer.o build/main.o -o build/CubeRenderer.exe -lopengl32 -lgdi32 -lglfw3 -L deps/glfw/lib-mingw-w64

if errorlevel 1 (
    echo.
    echo Compilation failed. Trying alternative linking...
    g++ build/glad.o build/Shader.o build/Camera.o build/CubeRenderer.o build/main.o -o build/CubeRenderer.exe -lopengl32 -lgdi32 -lglfw3
)

echo.
if exist build\CubeRenderer.exe (
    echo Build successful! Executable is at build\CubeRenderer.exe
) else (
    echo Build failed. Please check if GLFW library is properly installed.
)
pause