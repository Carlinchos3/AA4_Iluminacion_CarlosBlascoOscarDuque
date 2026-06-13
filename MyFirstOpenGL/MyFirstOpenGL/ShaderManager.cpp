#include "ShaderManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

ShaderManager::ShaderManager(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath) 
{
    GLuint vertexShader = LoadVertexShader(vertexPath);
    GLuint geometryShader = LoadGeometryShader(geometryPath);
    GLuint fragmentShader = LoadFragmentShader(fragmentPath);
    m_program = CreateProgram(vertexShader, geometryShader, fragmentShader);
}

void ShaderManager::Use() const 
{
    glUseProgram(m_program);
}

GLuint ShaderManager::GetProgram() const 
{
    return m_program;
}

std::string ShaderManager::LoadFile(const std::string& filePath) const 
{
    std::ifstream file(filePath);
    
    if (!file.is_open()) 
    {
        std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    std::string content, line;
    
    while (std::getline(file, line))
        content += line + "\n";
    
    return content;
}

GLuint ShaderManager::LoadVertexShader(const std::string& filePath) const 
{
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    std::string code = LoadFile(filePath);
    const char* src = code.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        std::cerr << "Error vertex shader: " << log.data() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return shader;
}

GLuint ShaderManager::LoadGeometryShader(const std::string& filePath) const 
{
    GLuint shader = glCreateShader(GL_GEOMETRY_SHADER);
    std::string code = LoadFile(filePath);
    const char* src = code.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        std::cerr << "Error geometry shader: " << log.data() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return shader;
}

GLuint ShaderManager::LoadFragmentShader(const std::string& filePath) const 
{
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string code = LoadFile(filePath);
    const char* src = code.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        std::cerr << "Error fragment shader: " << log.data() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return shader;
}

GLuint ShaderManager::CreateProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) const 
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, geometryShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) 
    {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        std::cerr << "Error al linkar programa: " << log.data() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, geometryShader);
    glDetachShader(program, fragmentShader);

    return program;
}