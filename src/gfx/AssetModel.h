//
// Created by sfdd on 6/25/23.
//

#pragma once

#include "Mapping.h"

#include <memory>
#include <unordered_map>

namespace edu::gfx
{

class Asset;

class AssetModel
{
public:
    using AssetMap = std::unordered_map<Mapping::AssetId, std::unique_ptr<Asset>>;

    AssetModel();
    ~AssetModel() = default;

    const Asset *asset(Mapping::AssetId assetId) const;
    Asset *asset(Mapping::AssetId assetId);

    uint64_t meshCount() const;

private:
    AssetMap m_assetMap;
    uint64_t m_totalMeshCount {0};
};

} // namespace edu::gfx