//
// Created by sfdd on 6/23/23.
//

#include "Mesh.h"

namespace edu::gfx
{

Mesh::Mesh(std::vector<VertexData> &&vertices, std::vector<uint32_t> &&indices, std::vector<Texture> &&textures)
    : vertices(std::move(vertices))
    , vertIndices(std::move(indices))
    , textures(std::move(textures))
{
}

} // namespace edu::gfx