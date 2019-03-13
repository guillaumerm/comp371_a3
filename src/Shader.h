#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderloader.h"

class Shader{
    public:
        Shader(const char * vertex_file_path, const char * fragment_file_path);
        void use();
        void setMat4(glm::mat4 matrix, const char *locationName);
        void setInt(int value, const char *locationName);
        void setFloat(float value, const char *locationName);
        void setVec3(glm::vec3 vec, const char *locationName);
        int getShaderID();
    private:
        GLuint m_shaderID;
};

#endif