//
// Created by sfdd on 5/29/23.
//

#include "Renderer.h"

#include "objects/Asset.h"
#include <gfx/AssetModel.h>
#include <gfx/Shader.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <numeric>

namespace edu::renderer
{

Renderer::Renderer(std::unique_ptr<gfx::AssetModel> assetModel)
    : m_assetModel(std::move(assetModel))
{
    const uint64_t totalVertCount {m_assetModel->meshCount()};

    // TODO: optimize when the number of assets becomes too much and this becomes too expensive
    m_VAOs.resize(totalVertCount);
    m_VBOs.resize(totalVertCount);
    m_EBOs.resize(totalVertCount);
    // Generate vertex array, vertex buffer, element buffer objects
    glGenVertexArrays(static_cast<int32_t>(totalVertCount), m_VAOs.data());
    glGenBuffers(static_cast<int32_t>(totalVertCount), m_VBOs.data());
    glGenBuffers(static_cast<int32_t>(totalVertCount), m_EBOs.data());
}

void Renderer::setCurrentObject(objects::Object *object)
{
    m_currentObject = object;

    if (!m_currentObject) {
        std::cout << "ERROR object with asset id " << static_cast<uint32_t>(m_currentObject->asset()->id())
                  << " was not found!" << std::endl;
        return;
    }

    m_currentObject->asset()->shader()->useShaderProgram();

    size_t index {0};
    for (const auto &mesh : m_currentObject->asset()->meshes()) {
        bindVertexArray(index);
        bindVertexBuffer(index);
        bindElementArray(index);
        setupMesh(mesh);
        ++index;
    }
}

void Renderer::onRenderStep()
{
    if (!m_currentObject) {
        std::cout << "Current asset is null on render step!\n";
        return;
    }

    const auto *const currentShader = m_currentObject->asset()->shader();
    currentShader->useShaderProgram();

    currentShader->setValue("u_View", m_viewMatrix);
    currentShader->setValue("u_Projection", m_perspectiveMatrix);

    // TODO: What about the order? This is not correct. Make a composition instead  of this and additionally multiply
    if (!m_currentObject->isQueued()) {
        currentShader->setValue("u_Model", m_modelMatrix);
        return;
    }

    // Perform transformations
    glm::mat4 transformation(1.0f);
    m_currentObject->composeTransformation(transformation);
    currentShader->setValue("u_Model", transformation * m_modelMatrix);
}

void Renderer::draw()
{
    if (!m_currentObject) {
        std::cout << "Current asset is null. Nothing is drawn!\n";
        return;
    }

    m_currentObject->asset()->shader()->useShaderProgram();

    size_t index {0};
    for (const auto &mesh : m_currentObject->asset()->meshes()) {
        bindVertexArray(index);
        bindTextures(mesh);
        glDrawElements(GL_TRIANGLES, static_cast<int32_t>(mesh.vertIndices.size()), GL_UNSIGNED_INT, nullptr);
        ++index;
    }
    // reset textures and vertex arrays
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(0);
}

void Renderer::clean()
{
    glDeleteVertexArrays(static_cast<int32_t>(m_VAOs.size()), m_VAOs.data());
    glDeleteBuffers(static_cast<int32_t>(m_VBOs.size()), m_VBOs.data());
    glDeleteBuffers(static_cast<int32_t>(m_EBOs.size()), m_EBOs.data());
    // TODO: How to clear all assets?
    //    glDeleteProgram(m_currentasset->shader()->programId());
}

void Renderer::setModelMatrix(const glm::mat4 &modelMatrix)
{
    m_modelMatrix = modelMatrix;
}

void Renderer::setViewMatrix(const glm::mat4 &viewMatrix)
{
    m_viewMatrix = viewMatrix;
}

void Renderer::setPerspectiveMatrix(const glm::mat4 &perspectiveMatrix)
{
    m_perspectiveMatrix = perspectiveMatrix;
}

void Renderer::setupMesh(const gfx::Mesh &mesh)
{
    setupBufferData(mesh);
    setupBufferPointers(mesh);
}

void Renderer::setupBufferData(const gfx::Mesh &mesh)
{
    const auto &vertices = mesh.vertices;
    const auto &indices = mesh.vertIndices;

    // Copy data in the static draw mode
    glBufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(vertices.size() * sizeof(gfx::Mesh::VertexData)),
                 vertices.data(), GL_STATIC_DRAW);
    // Copy data in the static draw mode
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int64_t>(indices.size() * sizeof(uint32_t)), indices.data(),
                 GL_STATIC_DRAW);
}

void Renderer::setupBufferPointers(const gfx::Mesh &mesh)
{
    {
        const auto &[layoutLocation, size] = mesh.vertexInfo.position;
        // vertex positions
        glEnableVertexAttribArray(layoutLocation);
        glVertexAttribPointer(layoutLocation, size, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::VertexData),
                              reinterpret_cast<void *>(offsetof(gfx::Mesh::VertexData, position)));
    }
    {
        const auto &[layoutLocation, size] = mesh.vertexInfo.normal;
        // vertex normals
        glEnableVertexAttribArray(layoutLocation);
        glVertexAttribPointer(layoutLocation, size, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::VertexData),
                              reinterpret_cast<void *>(offsetof(gfx::Mesh::VertexData, normal)));
    }
    {
        const auto &[layoutLocation, size] = mesh.vertexInfo.texture;
        // vertex texture coords
        glEnableVertexAttribArray(layoutLocation);
        glVertexAttribPointer(layoutLocation, size, GL_FLOAT, GL_FALSE, sizeof(gfx::Mesh::VertexData),
                              reinterpret_cast<void *>(offsetof(gfx::Mesh::VertexData, texCoords)));
    }
}

void Renderer::bindVertexArray(const size_t index)
{
    if (const auto vaoCount = m_VAOs.size(); index < vaoCount) {
        glBindVertexArray(m_VAOs[index]);
    } else {
        std::cout << "ERROR while binding the vertex array at index " << index << ": the number of arrays is "
                  << vaoCount << "!\n";
    }
}

void Renderer::bindVertexBuffer(const size_t index)
{
    if (const auto vboCount = m_VBOs.size(); index < vboCount) {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[index]);
    } else {
        std::cout << "ERROR while binding the vertex buffer at index " << index << ": the number of vertex buffers is "
                  << vboCount << "!\n";
    }
}

void Renderer::bindElementArray(const size_t index)
{
    if (const auto eboCount = m_EBOs.size(); index < eboCount) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOs[index]);
    } else {
        std::cout << "ERROR while binding the element buffer at index " << index
                  << ": the number of element buffers is " << eboCount << "!\n";
    }
}

void Renderer::bindTextures(const gfx::Mesh &mesh)
{
    if (!m_currentObject) {
        std::cout << "Current asset is null. Can't bind the textures!\n";
        return;
    }

    const auto *const shader = m_currentObject->asset()->shader();

    size_t index {0};
    for (const auto &texture : mesh.textures) {
        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + index);
        // Retrieve texture number (the N in diffuse_textureN)

        switch (texture.innerId()) {
        case gfx::Mapping::TextureId::HangingLightDiffuse:
        case gfx::Mapping::TextureId::BackpackDiffuse:
            shader->setValue("u_Material.diffuse", static_cast<int32_t>(index));
            break;
        case gfx::Mapping::TextureId::HangingLightSpecular:
        case gfx::Mapping::TextureId::BackpackSpecular:
            shader->setValue("u_Material.specular", static_cast<int32_t>(index));
            break;
        case gfx::Mapping::TextureId::Invalid:
            break;
        }

        glBindTexture(GL_TEXTURE_2D, texture.id());
        ++index;
    }
}

} // namespace edu::renderer
