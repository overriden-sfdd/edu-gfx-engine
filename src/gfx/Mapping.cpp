//
// Created by sfdd on 6/25/23.
//

#include "Mapping.h"

namespace edu::gfx
{

auto Mapping::AssetModelResource() -> const AssetModelMap &
{
    static const AssetModelMap assetModelMap {
        {
            AssetId::Backpack,
            {
                "data/backpack/backpack.obj",
                "data/shaders/general.vert",
                "data/shaders/general.frag",
            },
        },
        {
            AssetId::HangingLight,
            {
                "data/low_poly_hanging_light/hanging_light.obj",
                "data/shaders/general.vert",
                "data/shaders/general.frag",
            },
        },
    };

    return assetModelMap;
}

auto Mapping::TexResource() -> const TextureMap &
{

    static const TextureMap textureMap {
        {
            AssetId::Backpack,
            {
                "data/backpack/",
                {
                    TextureId::BackpackDiffuse,
                    TextureId::BackpackSpecular,
                },
            },
        },
        {
            AssetId::HangingLight,
            {
                "data/low_poly_hanging_light/",
                {
                    TextureId::HangingLightDiffuse,
                    TextureId::HangingLightSpecular,
                },
            },
        },
    };

    return textureMap;
}

} // namespace edu::gfx