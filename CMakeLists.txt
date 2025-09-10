cmake_minimum_required(VERSION 3.10)
project(CubeRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW subdirectory or find it
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw")
    add_subdirectory(deps/glfw)
else()
    find_package(glfw3 3.3 REQUIRED)
endif()

# Add GLAD
add_library(glad deps/glad/src/glad.c)
target_include_directories(glad PUBLIC deps/glad/include)

# Add GLM
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/glm")
    find_package(glm REQUIRED)
endif()

# Add Bullet Physics sources
file(GLOB BULLET_SOURCES 
    deps/bullet3/src/btBulletCollisionAll.cpp
    deps/bullet3/src/btBulletDynamicsAll.cpp
    deps/bullet3/src/btLinearMathAll.cpp
)

# Main executable with ECS
add_executable(CubeRendererECS 
    src/main_ecs.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/CubeRenderer.cpp
    src/ECS/Component.cpp
    src/ECS/PhysicsSystem.cpp
    ${BULLET_SOURCES}
)

target_include_directories(CubeRendererECS PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/bullet3/src
)

target_link_libraries(CubeRendererECS 
    OpenGL::GL
    glfw
    glad
)

# Original executable (without ECS)
add_executable(CubeRenderer 
    src/main.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/CubeRenderer.cpp
)

target_include_directories(CubeRenderer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/glm
)

target_link_libraries(CubeRenderer 
    OpenGL::GL
    glfw
    glad
)

# Copy shaders to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})