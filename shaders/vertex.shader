
#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;

//Used the logic from learnopengl.com
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 normal;
out vec3 fragmentPosition;

void main()
{
    // Calculate the normal
    normal = mat3(transpose(inverse(model))) * normals;

    fragmentPosition = vec3(model * vec4(position, 1.0f));
    
    gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);
}