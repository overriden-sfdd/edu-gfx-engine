//
// Created by sfdd on 6/10/23.
//

#pragma once

#include "interface/Transformable.h"

#include <glm/mat4x4.hpp>

#include <functional>
#include <memory>
#include <stack>

namespace edu::objects
{

class Asset;

class Object : public interface::Transformable
{
public:
    using TransformationType = glm::mat4;

    ~Object() override = default;

    void enqueue();
    void dequeue();
    [[nodiscard]] bool isQueued() const;

    void translate(const TranslationType &pos) override;
    void rotate(const RotationType &rot) override;
    void scale(const ScaleType &scaling) override;

    // TODO: is this a good approach? Is this a good place?
    void composeTransformation(TransformationType &transform);
    [[nodiscard]] const Asset *asset() const;
    void setAsset(std::shared_ptr<Asset> asset);

protected:
    // If the object is queued, the renderer will update it
    bool m_queued {false};
    std::shared_ptr<Asset> m_asset;

private:
    std::stack<std::function<void(glm::mat4 &)>> m_compositionStack;
};

} // namespace edu::objects
