@echo off
echo Building Bullet Physics library...

set GPP=C:\mingw64\bin\g++.exe
set AR=C:\mingw64\bin\ar.exe

if not exist "%GPP%" (
    echo Error: G++ not found at %GPP%
    pause
    exit /b 1
)

if not exist build\bullet mkdir build\bullet

echo Compiling Bullet LinearMath...
%GPP% -std=c++17 -O2 -c deps/bullet3/src/LinearMath/*.cpp -I deps/bullet3/src -o build/bullet/LinearMath.o 2>nul

echo Compiling Bullet Collision...
for %%f in (deps\bullet3\src\BulletCollision\BroadphaseCollision\*.cpp) do (
    %GPP% -std=c++17 -O2 -c "%%f" -I deps/bullet3/src -o "build\bullet\%%~nf.o" 2>nul
)
for %%f in (deps\bullet3\src\BulletCollision\CollisionDispatch\*.cpp) do (
    %GPP% -std=c++17 -O2 -c "%%f" -I deps/bullet3/src -o "build\bullet\%%~nf.o" 2>nul
)
for %%f in (deps\bullet3\src\BulletCollision\CollisionShapes\*.cpp) do (
    %GPP% -std=c++17 -O2 -c "%%f" -I deps/bullet3/src -o "build\bullet\%%~nf.o" 2>nul
)
for %%f in (deps\bullet3\src\BulletCollision\NarrowPhaseCollision\*.cpp) do (
    %GPP% -std=c++17 -O2 -c "%%f" -I deps/bullet3/src -o "build\bullet\%%~nf.o" 2>nul
)

echo Compiling Bullet Dynamics...
for %%f in (deps\bullet3\src\BulletDynamics\Dynamics\*.cpp) do (
    %GPP% -std=c++17 -O2 -c "%%f" -I deps/bullet3/src -o "build\bullet\%%~nf.o" 2>nul
)
for %%f in (deps\bullet3\src\BulletDynamics\ConstraintSolver\*.cpp) do (
    %GPP% -std=c++17 -O2 -c "%%f" -I deps/bullet3/src -o "build\bullet\%%~nf.o" 2>nul
)

echo Creating static library...
%AR% rcs build/bullet/libbullet.a build/bullet/*.o

echo Bullet Physics built successfully!
echo Library location: build/bullet/libbullet.a
pause