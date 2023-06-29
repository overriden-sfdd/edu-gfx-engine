//
// Created by sfdd on 6/25/23.
//

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <tuple>

namespace edu::gfx
{

class Mapping
{
public:
    enum class AssetId : uint32_t
    {
        Backpack,
        Test,
    };

    enum class TextureType : uint32_t
    {
        BackpackDiffuse,
        BackpackSpecular,
    };

    using AssetModelMap = std::map<AssetId, std::tuple<std::string, std::string, std::string>>;
    /** The map of asset model files, where key is the asset id and value is the path to the asset model
     */
    static const AssetModelMap &AssetModelResource();

    using TextureMap = std::map<TextureType, std::string>;
    /** The map of texture images, where key is the texture id and value is the path to the texture image
     */
    static const TextureMap &TexResource();
};

} // namespace edu::gfx
