//
// Created by sfdd on 6/23/23.
//

#pragma once

#include "Texture.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <vector>

namespace edu::gfx
{

class Mesh
{
public:
    struct VertexInfo
    {
        // {layout location, data size}
        using InfoPair = std::pair<uint32_t, int32_t>;
        InfoPair position {0, 3};
        InfoPair normal {1, 3};
        InfoPair texture {2, 2};
    };

    struct VertexData
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    Mesh(std::vector<VertexData> &&vertices_, std::vector<uint32_t> &&indices_, std::vector<Texture> &&textures_);

    VertexInfo vertexInfo;
    std::vector<VertexData> vertices;
    std::vector<uint32_t> vertIndices;
    std::vector<Texture> textures;
};

} // namespace edu::gfx
