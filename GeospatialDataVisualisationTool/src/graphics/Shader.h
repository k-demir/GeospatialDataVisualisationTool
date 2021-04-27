#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"


struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};


class Shader {
    unsigned int id_;
    std::string filepath_;
    std::unordered_map<std::string, int> uniformLocationCache_;
    
public:
    Shader(const std::string& filepath);
    ~Shader();
    void bind() const;
    void unbind() const;
    void setUniformVec4f(const std::string& name, const glm::vec4& vec);
    void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void setUniform1iv(const std::string& name, unsigned int len, int* val);
    void setUniform1fv(const std::string& name, unsigned int len, float* val);
    void setUniform1i(const std::string &name, int val);
    
private:
    unsigned int getUniformLocation(const std::string& name);
    ShaderProgramSource parseShader(const std::string& filepath);
    unsigned int compileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};
