//
// Created by sfdd on 6/23/23.
//

#pragma once

#include "Mapping.h"
#include "Mesh.h"

#include <objects/Object.h>

#include <assimp/scene.h>

#include <memory>
#include <unordered_set>

namespace edu::gfx
{

class Shader;
class Texture;

class Asset : public objects::Object
{
public:
    Asset(Mapping::AssetId id, const std::string &path, std::unique_ptr<Shader> shader);
    ~Asset() override = default;

    const std::vector<Mesh> &meshes() const;
    const Shader *const shader() const;
    Mapping::AssetId id() const;

private:
    void loadModel(const std::string &path);
    void processNode(const aiNode *node, const aiScene *scene);
    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(std::vector<Texture> &out, const aiMaterial *mat, aiTextureType type,
                              const std::string &textureDir, Mapping::TextureId textureId);

    // Texture cache
    std::unordered_set<Mapping::TextureId> m_loadedTextures;

    Mapping::AssetId m_id;

    std::unique_ptr<Shader> m_shader;
    std::vector<Mesh> m_meshes;
};

} // namespace edu::gfx