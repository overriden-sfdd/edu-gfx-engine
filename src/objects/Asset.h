//
// Created by sfdd on 6/23/23.
//

#pragma once

#include "Object.h"

#include <gfx/Mapping.h>
#include <gfx/Mesh.h>

#include <assimp/scene.h>

#include <memory>
#include <unordered_set>

namespace edu::gfx
{
class Shader;
class Texture;
} // namespace edu::gfx

namespace edu::objects
{

class Asset
{
public:
    Asset(gfx::Mapping::AssetId id, const std::string &path, std::shared_ptr<gfx::Shader> shader);
    ~Asset() = default;

    const std::vector<gfx::Mesh> &meshes() const;
    const gfx::Shader *shader() const;
    gfx::Mapping::AssetId id() const;

private:
    void loadModel(const std::string &path);
    void processNode(const aiNode *node, const aiScene *scene);
    gfx::Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(std::vector<gfx::Texture> &out, const aiMaterial *mat, aiTextureType type,
                              const std::string &textureDir, gfx::Mapping::TextureId textureId);

    // Texture cache
    std::unordered_set<gfx::Mapping::TextureId> m_loadedTextures;

    gfx::Mapping::AssetId m_id;

    std::shared_ptr<gfx::Shader> m_shader;
    std::vector<gfx::Mesh> m_meshes;
};

} // namespace edu::objects