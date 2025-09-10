@echo off
echo Setting up OpenGL dependencies...

mkdir deps 2>nul
cd deps

echo Downloading GLFW...
git clone https://github.com/glfw/glfw.git

echo Downloading GLM...
git clone https://github.com/g-truc/glm.git

echo Downloading GLAD...
mkdir glad
cd glad
mkdir include
mkdir src
cd ..

echo.
echo Please download GLAD from https://glad.dav1d.de/ with the following settings:
echo - Language: C/C++
echo - Specification: OpenGL
echo - API gl: Version 3.3
echo - Profile: Core
echo - Generate a loader: checked
echo.
echo Extract the files into deps/glad/ (include and src folders)
echo.
cd ..
echo Dependencies setup complete!