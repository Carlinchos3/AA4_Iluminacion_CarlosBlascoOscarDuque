#pragma once
#include <GL/glew.h>
#include <string>

class Texture {
public:
    Texture() : textureID(0) {}
    Texture(const std::string& filePath);
    void Bind() const;

private:
    GLuint textureID;
};