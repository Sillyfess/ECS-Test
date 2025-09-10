# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

OpenGL 3D renderer with Entity Component System (ECS) architecture for displaying animated cubes using modern C++17.

## Build Commands

### Windows with MinGW64
```bash
# Build ECS version (recommended)
compile_ecs.bat

# Build non-ECS version
compile.bat

# Run the ECS executable
build\CubeRendererECS.exe
```

### CMake Build
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Dependency Setup
```bash
# Clone GLFW and GLM dependencies
setup_deps.bat

# GLAD must be manually downloaded from https://glad.dav1d.de/
# Settings: gl Version 3.3 Core, C/C++, Generate loader
# Extract to deps/glad/
```

## Architecture

### ECS Core (include/ECS/)
The project uses a custom Entity Component System with:
- **World**: Container managing entities, components, and systems
- **Entity**: Simple ID-based containers with component masks
- **Component**: Data-only structures (Transform, Velocity, Renderable, Input, Tag)
- **System**: Logic processors that operate on entities with specific components

### System Execution Order
Systems run in priority order (lower values execute first):
1. InputSystem (-100): Processes GLFW keyboard/mouse events
2. PlayerControllerSystem (-50): Converts input to player movement
3. MovementSystem (0): Updates positions based on velocity
4. BoundsSystem (10): Enforces world boundaries
5. RenderSystem (100): Prepares rendering data

### Key Architectural Patterns
- **Component Masks**: Bitset-based entity queries for efficient system filtering
- **Instanced Rendering**: All cubes rendered in single draw call using instance matrices
- **System Communication**: Systems communicate via shared components (e.g., Input → Velocity)
- **Template-based Component Registration**: Type-safe component access via templates

### Rendering Pipeline
1. RenderSystem collects Transform and Renderable components
2. Instance matrices built from entity transforms
3. CubeRenderer performs instanced draw with per-instance colors
4. Phong lighting applied in fragment shader

## Controls

| Key | Action |
|-----|--------|
| W/A/S/D | Move forward/left/backward/right |
| Space | Jump/move up |
| Shift | Move down |
| Q/E or Arrows | Rotate player |
| Right Mouse | Camera look |
| 1 | Spawn cube at player |
| 2 | Remove random cube |
| 3 | Toggle cube spinning |
| ESC | Exit |

## Code Conventions

- **Headers**: `.h` files in `include/`, implementation in `src/`
- **Namespaces**: ECS code under `ECS::` namespace
- **Smart Pointers**: `std::shared_ptr` for entities, `std::unique_ptr` for components/systems
- **GLM Math**: Use `glm::vec3`, `glm::quat`, `glm::mat4` for 3D math
- **OpenGL**: Raw OpenGL calls wrapped in renderer classes

## Testing

No automated tests currently exist. Manual testing via:
```bash
# Run and verify rendering/controls work
build\CubeRendererECS.exe
```

## Common Development Tasks

### Adding New Component
```cpp
// In include/ECS/Components/NewComponent.h
class NewComponent : public ECS::Component {
public:
    float value;
    NewComponent(float v) : value(v) {}
};
```

### Adding New System
```cpp
// In include/ECS/Systems/NewSystem.h
class NewSystem : public ECS::System {
public:
    NewSystem() {
        RequireComponents<Transform, NewComponent>();
    }
    void Update(float deltaTime) override {
        for (auto& entity : GetEntities()) {
            // System logic
        }
    }
};
```

### Creating Entities
```cpp
auto entity = world.CreateEntity();
world.AddComponent<Transform>(entity, Transform(position));
world.AddComponent<Renderable>(entity, Renderable(MeshType::Cube, color));
```