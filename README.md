# OpenGL 1000 Cubes Renderer

A high-performance OpenGL renderer that displays 1000 animated cubes using instanced rendering.

## Features
- Instanced rendering for efficient display of 1000 cubes
- Per-instance coloring based on instance ID
- Phong lighting with ambient, diffuse, and specular components
- Free-flying camera with mouse and keyboard controls
- Smooth cube rotation animations
- Random positioning and scaling of cubes

## Controls
- **W/S** - Move forward/backward
- **A/D** - Move left/right
- **Space** - Move up
- **Left Shift** - Move down
- **Mouse** - Look around
- **Scroll** - Zoom in/out
- **ESC** - Exit

## Building

### Prerequisites
- CMake 3.10+
- C++17 compatible compiler
- OpenGL 3.3+ support

### Setup Dependencies
1. Run `setup_deps.bat` to clone GLFW and GLM
2. Download GLAD from https://glad.dav1d.de/ with:
   - Language: C/C++
   - API: gl Version 3.3 Core
   - Generate a loader: checked
3. Extract GLAD files to `deps/glad/`

### Build
Run `build.bat` or:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Technical Details
- Uses modern OpenGL 3.3 Core Profile
- Instanced rendering with per-instance transformation matrices
- Efficient buffer updates for animation
- Vertex shader calculates per-instance colors procedurally
- Fragment shader implements Phong lighting model