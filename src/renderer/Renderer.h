//
// Created by sfdd on 5/20/23.
//

#pragma once

#include <gfx/Mapping.h>

#include <glm/matrix.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace edu::objects
{
class Object;
} // namespace edu::objects

namespace edu::gfx
{
class Mesh;
class AssetModel;
} // namespace edu::gfx

namespace edu::renderer
{

class Renderer
{
public:
    explicit Renderer(std::unique_ptr<gfx::AssetModel> assetModel);
    ~Renderer() = default;

    void setCurrentObject(objects::Object *object);
    void onRenderStep();
    void draw();
    void clean();

    void setModelMatrix(const glm::mat4 &modelMatrix);
    void setViewMatrix(const glm::mat4 &viewMatrix);
    void setPerspectiveMatrix(const glm::mat4 &perspectiveMatrix);

private:
    void setupMesh(const gfx::Mesh &mesh);
    void setupBufferData(const gfx::Mesh &mesh);
    void setupBufferPointers(const gfx::Mesh &mesh);

    void bindVertexArray(size_t index);
    void bindVertexBuffer(size_t index);
    void bindElementArray(size_t index);
    // Binds all texture for a mesh
    void bindTextures(const gfx::Mesh &mesh);

    std::vector<uint32_t> m_VAOs;
    std::vector<uint32_t> m_VBOs;
    std::vector<uint32_t> m_EBOs;

    // TODO: remove this temporary solution
public:
    std::unique_ptr<gfx::AssetModel> m_assetModel;

private:
    objects::Object *m_currentObject {nullptr};

    // MVP matrices
    glm::mat4 m_modelMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_perspectiveMatrix;
};

} // namespace edu::renderer
