//
// Created by sfdd on 6/25/23.
//

#pragma once

#include <cstdint>
#include <string>
#include <tuple>
#include <unordered_map>

namespace edu::gfx
{

class Mapping
{
public:
    enum class AssetId : uint32_t
    {
        Invalid = 0,
        Backpack,
        HangingLight
    };

    enum class TextureId : uint32_t
    {
        Invalid = 0,
        BackpackDiffuse,
        BackpackSpecular,
        HangingLightDiffuse,
        HangingLightSpecular
    };

    using AssetModelMap = std::unordered_map<AssetId, std::tuple<std::string, std::string, std::string>>;
    /**
     * The map of asset model files, where key is the asset id and value is the path to the asset model
     */
    static const AssetModelMap &AssetModelResource();

    using TextureMap = std::unordered_map<AssetId, std::pair<std::string, std::array<TextureId, 2>>>;
    /**
     * The map of textures where key is the asset id, value is the pair {path to the texture files, array of texture ids}.
     * The array has the following order {Diffuse, Specular}
     */
    static const TextureMap &TexResource();
};

} // namespace edu::gfx
