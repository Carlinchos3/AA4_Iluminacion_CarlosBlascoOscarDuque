#pragma once
#include <GL/glew.h>
#include <string>

class ShaderManager 
{
public:
    ShaderManager(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);
    void Use() const;
    GLuint GetProgram() const;

private:
    GLuint program;

    std::string LoadFile(const std::string& filePath) const;
    GLuint LoadVertexShader(const std::string& filePath) const;
    GLuint LoadGeometryShader(const std::string& filePath) const;
    GLuint LoadFragmentShader(const std::string& filePath) const;
    GLuint CreateProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) const;
};