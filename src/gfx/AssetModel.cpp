//
// Created by sfdd on 6/25/23.
//

#include "AssetModel.h"
#include "Asset.h"
#include "Shader.h"

namespace edu::gfx
{

AssetModel::AssetModel()
{
    for (const auto &[assetId, pathTuple] : Mapping::AssetModelResource()) {
        const auto &[assetModelPath, vertexShaderPath, fragShaderPath] = pathTuple;
        m_assetMap[assetId] = std::make_unique<Asset>(assetId, assetModelPath,
                                                      std::make_unique<Shader>(vertexShaderPath, fragShaderPath));
    }
}

const Asset *AssetModel::asset(Mapping::AssetId assetId) const
{
    const auto it = m_assetMap.find(assetId);
    return it == m_assetMap.cend() ? nullptr : it->second.get();
}

} // namespace edu::gfx
