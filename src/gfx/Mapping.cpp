//
// Created by sfdd on 6/25/23.
//

#include "Mapping.h"

namespace edu::gfx
{

auto Mapping::AssetModelResource() -> const AssetModelMap &
{
    static const AssetModelMap assetModelMap {
        {AssetId::Backpack, {"data/backpack/backpack.obj", "data/shaders/test.vert", "data/shaders/test.frag"}},
    };

    return assetModelMap;
}

auto Mapping::TexResource() -> const TextureMap &
{
    static const TextureMap textureMap {
        {TextureType::BackpackDiffuse, "data/backpack/diffuse.jpg"},
        {TextureType::BackpackSpecular, "data/backpack/specular.jpg"},
    };

    return textureMap;
}

} // namespace edu::gfx