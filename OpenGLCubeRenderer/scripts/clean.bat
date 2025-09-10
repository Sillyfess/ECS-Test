@echo off
echo Cleaning build files...
echo ======================

set PROJECT_DIR=%~dp0..
cd /d "%PROJECT_DIR%"

if exist build (
    echo Removing build directory...
    rmdir /S /Q build
    echo Clean complete!
) else (
    echo Nothing to clean.
)

pause