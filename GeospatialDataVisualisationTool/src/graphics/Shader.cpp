#include "Shader.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


Shader::Shader(const std::string& filepath) : filepath_(filepath), id_(0) {
    ShaderProgramSource source = parseShader(filepath);
    id_ = createShader(source.vertexSource, source.fragmentSource);
}


Shader::~Shader() { 
    glDeleteProgram(id_);
}


void Shader::bind() const { 
    glUseProgram(id_);
}


void Shader::unbind() const { 
    glUseProgram(0);
}


void Shader::setUniformVec4f(const std::string& name, const glm::vec4& vec) {
    glUniform4f(getUniformLocation(name), vec[0], vec[1], vec[2], vec[3]);
}


void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}


void Shader::setUniform1iv(const std::string& name, unsigned int len, int* val) {
    glUniform1iv(getUniformLocation(name), len, val);
}


void Shader::setUniform1fv(const std::string& name, unsigned int len, float* val) {
    glUniform1fv(getUniformLocation(name), len, val);
}


void Shader::setUniform1i(const std::string &name, int val) {
    glUniform1f(getUniformLocation(name), val);
}

unsigned int Shader::getUniformLocation(const std::string& name) {
    if (uniformLocationCache_.find(name) != uniformLocationCache_.end())
        return uniformLocationCache_[name];
    
    int location = glGetUniformLocation(id_, name.c_str());
    if (location == -1)
        std::cout << "Uniform '" << name << "' does not exist!" << std::endl;

    uniformLocationCache_[name] = location;
    return location;
}


ShaderProgramSource Shader::parseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    
    while(getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else {
            ss[(int)type] << line << '\n';
        }
    }
    
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}
