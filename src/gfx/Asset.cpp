//
// Created by sfdd on 6/23/23.
//

#include "Asset.h"
#include "Shader.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

namespace edu::gfx
{

Asset::Asset(const Mapping::AssetId id, const std::string &path, std::unique_ptr<Shader> shader)
    : m_id(id)
    , m_shader(std::move(shader))
{
    m_shader->useShaderProgram();
    loadModel(path);
}

const std::vector<Mesh> &Asset::meshes() const
{
    return m_meshes;
}

const Shader *const Asset::shader() const
{
    return m_shader.get();
}

Mapping::AssetId Asset::id() const
{
    return m_id;
}

void Asset::loadModel(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        return;
    }
    processNode(scene->mRootNode, scene);
}

void Asset::processNode(const aiNode *const node, const aiScene *const scene)
{
    // First process all meshes of the node
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.emplace_back(processMesh(mesh, scene));
    }

    if (node->mNumChildren == 0) {
        return;
    }

    // Process all children of the node recursively
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Asset::processMesh(const aiMesh *const mesh, const aiScene *const scene)
{
    std::vector<Mesh::VertexData> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    // We only care about the first texture coordinates for a vertex
    const auto *const rawTex = mesh->mTextureCoords[0];
    const bool meshHasTexture = rawTex != nullptr;

    // Get positions and normals
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        const auto &vertPos = mesh->mVertices[i];

        glm::vec3 normal {0.f, 0.f, 0.f};
        if (mesh->HasNormals()) {
            const auto &rawNormal = mesh->mNormals[i];
            normal[0] = rawNormal.x;
            normal[1] = rawNormal.y;
            normal[2] = rawNormal.z;
        }

        glm::vec2 texCoords {0.f, 0.f};
        if (meshHasTexture) {
            texCoords[0] = rawTex[i].x;
            texCoords[1] = rawTex[i].y;
        }

        vertices.emplace_back(Mesh::VertexData {
            .position = {vertPos.x, vertPos.y, vertPos.z},
            .normal = normal,
            .texCoords = texCoords,
        });
    }

    // Process indices
    // Each face is a primitive (triangle in our case) with indices that specify the order
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process material
    if (mesh->mMaterialIndex >= 0) {
        const aiMaterial *const material = scene->mMaterials[mesh->mMaterialIndex];

        const auto assetTexIt = Mapping::TexResource().find(m_id);
        if (assetTexIt != Mapping::TextureMap().cend()) {
            const auto &[textureDir, textureIds] = assetTexIt->second;
            loadMaterialTextures(textures, material, aiTextureType_DIFFUSE, textureDir, textureIds.at(0));
            loadMaterialTextures(textures, material, aiTextureType_SPECULAR, textureDir, textureIds.at(1));
        } else {
            std::cout << "Couldn't find the texture for the asset with id: " << static_cast<uint32_t>(m_id) << '\n';
        }
    }

    return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

void Asset::loadMaterialTextures(std::vector<Texture> &out, const aiMaterial *const mat, const aiTextureType type,
                                 const std::string &textureDir, const Mapping::TextureId textureId)
{
    for (size_t i = 0; i < mat->GetTextureCount(type); ++i) {
        if (m_loadedTextures.find(textureId) == m_loadedTextures.cend()) {
            aiString str;
            mat->GetTexture(type, i, &str);
            auto texture = Texture(textureId, textureDir + str.C_Str());
            m_loadedTextures.insert(textureId);
            out.emplace_back(std::move(texture));
        }
    }
}

} // namespace edu::gfx