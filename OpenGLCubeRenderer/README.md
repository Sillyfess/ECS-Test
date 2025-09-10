# OpenGL Cube Renderer - 1000 Cubes Demo

A high-performance OpenGL 3.3 renderer displaying 1000 animated cubes using instanced rendering.

## 📁 Project Structure

```
OpenGLCubeRenderer/
├── src/                 # Source files
│   ├── main.cpp        # Application entry point
│   ├── Shader.cpp      # Shader loader/manager
│   ├── Camera.cpp      # Free-flying camera
│   └── CubeRenderer.cpp # Instanced cube rendering
├── include/            # Header files
│   ├── Shader.h
│   ├── Camera.h
│   └── CubeRenderer.h
├── shaders/            # GLSL shaders
│   ├── cube.vert      # Vertex shader
│   └── cube.frag      # Fragment shader
├── deps/               # Dependencies
│   ├── glad/          # OpenGL loader
│   ├── glfw/          # Window/context management
│   └── glm/           # Math library
├── scripts/            # Build scripts
│   ├── build.bat      # Compile the project
│   ├── run.bat        # Run the executable
│   └── clean.bat      # Clean build files
└── build/              # Build output (generated)
    └── CubeRenderer.exe

```

## 🚀 Quick Start

### Build
```bash
cd OpenGLCubeRenderer/scripts
build.bat
```

### Run
```bash
run.bat
```

Or manually:
```bash
cd OpenGLCubeRenderer/build
CubeRenderer.exe
```

## 🎮 Controls

- **W/A/S/D** - Move forward/left/backward/right
- **Space** - Move up
- **Shift** - Move down
- **Mouse** - Look around
- **Scroll** - Zoom in/out
- **ESC** - Exit

## ✨ Features

- **1000 Animated Cubes** - Each with unique colors and transformations
- **Instanced Rendering** - Efficient GPU-based rendering using a single draw call
- **Phong Lighting** - Ambient, diffuse, and specular lighting
- **Free Camera** - Full 3D movement and rotation
- **Procedural Colors** - Each cube has a unique color based on its instance ID
- **Smooth Animation** - Continuous rotation with varied speeds

## 🛠️ Technical Details

- **OpenGL Version**: 3.3 Core Profile
- **Rendering Method**: Instanced rendering with per-instance matrices
- **Shading Model**: Phong illumination
- **Dependencies**: GLFW 3.3, GLAD, GLM
- **Compiler**: MinGW-w64 (GCC)

## 📋 Requirements

- Windows OS
- MinGW-w64 installed at `C:\mingw64\`
- OpenGL 3.3+ compatible graphics card
- Graphics drivers with OpenGL support

## 🔧 Troubleshooting

If the build fails:
1. Ensure MinGW-w64 is installed at `C:\mingw64\`
2. Update the compiler path in `scripts/build.bat` if needed
3. Make sure all dependencies are in the `deps/` folder

## 📊 Performance

The renderer uses instanced rendering to draw all 1000 cubes in a single draw call, making it highly efficient even on modest hardware. Each cube's transformation matrix is updated per frame on the CPU and uploaded to the GPU as a buffer.

## 🎨 Customization

- **Cube Count**: Change `CUBE_COUNT` in `src/main.cpp`
- **Colors**: Modify the color generation in `shaders/cube.vert`
- **Lighting**: Adjust lighting parameters in `src/main.cpp` and `shaders/cube.frag`
- **Animation Speed**: Modify rotation speed in the main loop