#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 aTexCoords;

out vec3 normal;
out vec3 fragmentPosition;
out vec2 texCoord;
out vec4 fragmentPositionLightSpace;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;

void main() {
    // Calculate the normal
    normal = mat3(transpose(inverse(model))) * normals;
    
    texCoord = aTexCoords;
    
    fragmentPosition = vec3(model * vec4(position, 1.0f));
    
    fragmentPositionLightSpace = lightSpaceMatrix * vec4(fragmentPosition, 1.0f);

    gl_Position = projection * view * model * vec4(position, 1.0f);
}