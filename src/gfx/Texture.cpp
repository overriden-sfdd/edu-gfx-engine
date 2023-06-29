//
// Created by sfdd on 5/20/23.
//

#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

namespace edu::gfx
{

Texture::Texture(const Mapping::TextureType textureType)
    : m_textureType(textureType)
{
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    const auto resIt = Mapping::TexResource().find(textureType);
    if (resIt != Mapping::TexResource().cend()) {
        m_textureImageInfo.imagePath = resIt->second;
    } else {
        std::cout << "ERROR couldn't find the texture resource with type: " << static_cast<uint32_t>(textureType)
                  << '\n';
    }
    generateTexture();
    setTextureOptions();
}

int32_t Texture::imageFormat() const
{
    switch (m_textureImageInfo.channels) {
    case 1:
        return GL_RED;
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    default:
        std::cout << "WARNING There are " << m_textureImageInfo.channels
                  << " image channels which doesn't match any of the default cases. Falling back to GL_BLUE...";
        return GL_BLUE;
    }
}

uint32_t Texture::id() const
{
    return m_textureId;
}

Mapping::TextureType Texture::type() const
{
    return m_textureType;
}

void Texture::setTextureOptions() const
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::generateTexture()
{
    uint8_t *imageData {nullptr};
    loadImage(&imageData);
    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_textureImageInfo.width, m_textureImageInfo.height, 0, imageFormat(),
                     GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(imageData);
}

void Texture::loadImage(uint8_t **const imageData)
{
    stbi_set_flip_vertically_on_load(m_textureImageInfo.flipImage);
    *imageData = stbi_load(m_textureImageInfo.imagePath.c_str(), &m_textureImageInfo.width, &m_textureImageInfo.height,
                           &m_textureImageInfo.channels, 0);
}

} // namespace edu::gfx