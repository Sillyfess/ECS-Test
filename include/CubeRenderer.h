#ifndef CUBE_RENDERER_H
#define CUBE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;

class CubeRenderer {
public:
    CubeRenderer();
    ~CubeRenderer();
    
    void initialize();
    void render(const Shader& shader, const std::vector<glm::mat4>& modelMatrices);
    void cleanup();

private:
    unsigned int VAO, VBO, instanceVBO;
    void setupCube();
};

#endif