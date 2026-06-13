#pragma once
#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture(const std::string& filePath);
    void Bind() const;

private:
    GLuint m_textureID;
};