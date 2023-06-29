//
// Created by sfdd on 5/20/23.
//

#pragma once

#include "Mapping.h"

#include <cstdint>
#include <string>

namespace edu::gfx
{

class Texture
{
public:
    explicit Texture(Mapping::TextureType textureType);

    Texture(Texture &&) = default;
    Texture &operator=(Texture &&) = default;

    int32_t imageFormat() const;
    uint32_t id() const;
    Mapping::TextureType type() const;

private:
    struct ImageInfo
    {
        int32_t width {};
        int32_t height {};
        int32_t channels {};
        std::string imagePath {};
        bool flipImage {true};
    };

    void setTextureOptions() const;

    void generateTexture();
    void loadImage(uint8_t **const imageData);

    ImageInfo m_textureImageInfo;
    uint32_t m_textureId;
    Mapping::TextureType m_textureType;
};

} // namespace edu::gfx
