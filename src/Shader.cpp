#include "Shader.h"

Shader::Shader(const char * vertex_file_path, const char * fragment_file_path){
    this->m_shaderID = loadSHADER(vertex_file_path, fragment_file_path);
}

void Shader::use() {
    glUseProgram(m_shaderID);
}

void Shader::setMat4(glm::mat4 matrix, const char *locationName) {
    glUniformMatrix4fv(glGetUniformLocation(this->m_shaderID, locationName), 1, GL_FALSE, glm::value_ptr(matrix));
};

void Shader::setInt(int value, const char *locationName) {
    glUniform1i(glGetUniformLocation(this->m_shaderID, locationName), value);
};

void Shader::setFloat(float value, const char *locationName) {
    glUniform1f(glGetUniformLocation(this->m_shaderID, locationName), value);
};

void Shader::setVec3(glm::vec3 vec, const char *locationName) {
    glUniform3fv(glGetUniformLocation(this->m_shaderID, locationName), 1, glm::value_ptr(vec));
};

int Shader::getShaderID() {
    return this->m_shaderID;
}