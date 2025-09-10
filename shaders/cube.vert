#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in mat4 aInstanceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;

uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
    
    float r = sin(gl_InstanceID * 0.11) * 0.5 + 0.5;
    float g = cos(gl_InstanceID * 0.13) * 0.5 + 0.5;
    float b = sin(gl_InstanceID * 0.17 + 1.57) * 0.5 + 0.5;
    Color = vec3(r, g, b);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}