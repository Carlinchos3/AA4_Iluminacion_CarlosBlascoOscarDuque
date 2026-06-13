#include "Texture.h"
#include <stb_image.h>
#include <iostream>

Texture::Texture(const std::string& filePath) 
{
    int width, height, nrChannels;
    unsigned char* textureInfo = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (!textureInfo) {
        std::cerr << "Error al cargar textura: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureInfo);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(textureInfo);
}

void Texture::Bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}