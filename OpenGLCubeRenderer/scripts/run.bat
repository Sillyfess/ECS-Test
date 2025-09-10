@echo off
echo Starting OpenGL Cube Renderer...
echo ================================

set PROJECT_DIR=%~dp0..
cd /d "%PROJECT_DIR%\build"

if not exist CubeRenderer.exe (
    echo Error: CubeRenderer.exe not found!
    echo Please run build.bat first.
    pause
    exit /b 1
)

echo Launching renderer...
echo.
echo Controls:
echo   WASD        - Move camera
echo   Mouse       - Look around
echo   Space/Shift - Move up/down
echo   Scroll      - Zoom in/out
echo   ESC         - Exit
echo.

CubeRenderer.exe

pause