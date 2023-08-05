//
// Created by sfdd on 6/23/23.
//

#include "Mesh.h"

namespace edu::gfx
{

Mesh::Mesh(std::vector<VertexData> &&vertices_, std::vector<uint32_t> &&indices_, std::vector<Texture> &&textures_)
    : vertices(std::move(vertices_))
    , vertIndices(std::move(indices_))
    , textures(std::move(textures_))
{
}

} // namespace edu::gfx