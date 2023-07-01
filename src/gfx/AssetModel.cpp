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
        auto assetPtr = std::make_unique<Asset>(assetId, assetModelPath,
                                                std::make_unique<Shader>(vertexShaderPath, fragShaderPath));
        m_totalMeshCount += assetPtr->meshes().size();
        m_assetMap[assetId] = std::move(assetPtr);
    }
}

const Asset *AssetModel::asset(Mapping::AssetId assetId) const
{
    const auto it = m_assetMap.find(assetId);
    return it == m_assetMap.cend() ? nullptr : it->second.get();
}

uint64_t AssetModel::meshCount() const
{
    return m_totalMeshCount;
}

} // namespace edu::gfx
