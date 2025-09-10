# OpenGL ECS Cube Renderer

This project integrates an Entity Component System (ECS) with an OpenGL renderer, providing a flexible architecture for 3D applications with input handling.

## Features

### ECS Architecture
- **Entity-Component-System Pattern**: Clean separation of data (Components) from logic (Systems)
- **Components**: Transform, Velocity, Renderable, Input, Tag
- **Systems**: InputSystem, PlayerControllerSystem, MovementSystem, BoundsSystem, RenderSystem
- **Flexible Entity Management**: Dynamic creation and destruction of entities

### Input Handling
- **Keyboard Input**: WASD movement, Space for jump, Shift for descend, Q/E for rotation
- **Mouse Input**: Right-click drag for camera look
- **Interactive Features**:
  - Press `1` to spawn new cubes at player position
  - Press `2` to remove random cubes
  - Press `3` to toggle cube spinning

### Rendering
- **OpenGL 3.3 Core Profile**: Modern OpenGL rendering pipeline
- **Instanced Rendering**: Efficient rendering of multiple cubes
- **Dynamic Lighting**: Phong lighting model with ambient, diffuse, and specular components
- **Color Support**: Per-entity color customization

## Project Structure

```
ECS-Test/
├── include/
│   ├── ECS/
│   │   ├── Component.h         # Base component class
│   │   ├── Entity.h            # Entity management
│   │   ├── System.h            # Base system class
│   │   ├── World.h             # ECS world container
│   │   ├── Components/
│   │   │   ├── Transform.h     # Position, rotation, scale
│   │   │   ├── Velocity.h      # Linear and angular velocity
│   │   │   ├── Renderable.h    # Rendering properties
│   │   │   ├── Input.h         # Input state
│   │   │   └── Tag.h           # Entity tagging
│   │   └── Systems/
│   │       ├── InputSystem.h           # Input processing
│   │       ├── PlayerControllerSystem.h # Player control
│   │       ├── MovementSystem.h         # Physics movement
│   │       ├── BoundsSystem.h           # World boundaries
│   │       └── RenderSystem.h           # Rendering bridge
│   ├── Camera.h
│   ├── Shader.h
│   └── CubeRenderer.h
├── src/
│   ├── main_ecs.cpp           # Main application with ECS
│   ├── ECS/
│   │   └── Component.cpp      # Component implementation
│   ├── Camera.cpp
│   ├── Shader.cpp
│   └── CubeRenderer.cpp
└── shaders/
    ├── cube.vert              # Vertex shader
    └── cube.frag              # Fragment shader
```

## Building

### Prerequisites
- MinGW64 with C++17 support
- OpenGL 3.3+ compatible graphics driver
- GLFW3 library
- GLM (OpenGL Mathematics)

### Compilation
```bash
# Using the provided batch file
compile_ecs.bat

# Or manually with CMake
mkdir build
cd build
cmake ..
make
```

## Running

```bash
build\CubeRendererECS.exe
```

## Controls

| Key/Input | Action |
|-----------|--------|
| W/A/S/D | Move player forward/left/backward/right |
| Space | Jump |
| Shift | Move down |
| Q/E or Arrow Keys | Rotate player |
| Right Mouse + Move | Look around |
| 1 | Spawn new cube |
| 2 | Remove random cube |
| 3 | Toggle cube spinning |
| ESC | Exit |

## Architecture Details

### ECS Components
- **Transform**: Stores position (vec3), rotation (quaternion), and scale (vec3)
- **Velocity**: Contains linear and angular velocity vectors
- **Renderable**: Defines mesh type, color, visibility, and opacity
- **Input**: Tracks keyboard/mouse state and input events
- **Tag**: String identifier for entity categorization

### ECS Systems
1. **InputSystem** (Priority: -100): Processes GLFW input events
2. **PlayerControllerSystem** (Priority: -50): Translates input to player movement
3. **MovementSystem** (Priority: 0): Updates entity positions based on velocity
4. **BoundsSystem** (Priority: 10): Enforces world boundaries with bounce/wrap
5. **RenderSystem** (Priority: 100): Prepares entities for OpenGL rendering

### Game Loop
```cpp
while (!glfwWindowShouldClose(window)) {
    // Calculate delta time
    // Process window events
    // Update ECS systems
    world.Update(deltaTime);
    // Clear buffers
    // Render entities
    renderSystem->Render();
    // Swap buffers
}
```

## Extending the System

### Adding New Components
```cpp
class Health : public ECS::Component {
public:
    float current;
    float max;
    Health(float maxHealth) : current(maxHealth), max(maxHealth) {}
};
```

### Adding New Systems
```cpp
class HealthSystem : public ECS::System {
public:
    HealthSystem() {
        RequireComponents<Health, Transform>();
    }
    
    void Update(float deltaTime) override {
        // System logic here
    }
};
```

### Creating Entities
```cpp
auto entity = world.CreateEntity();
world.AddComponent<Transform>(entity, Transform(position));
world.AddComponent<Velocity>(entity, Velocity(linearVel));
world.AddComponent<Renderable>(entity, Renderable(MeshType::Cube, color));
```

## Performance Considerations
- Uses component masks for efficient entity queries
- Instanced rendering for multiple cubes
- Systems run in priority order for optimal data flow
- Entities are pooled and reused when possible

## Future Enhancements
- Collision detection system
- Particle effects system
- Audio component and system
- Networking support
- Save/load functionality
- More mesh types (sphere, plane, custom models)
- Texture support
- Shadow mapping