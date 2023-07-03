//
// Created by sfdd on 6/10/23.
//

#pragma once

#include "IObject.h"
#include "interface/Transformable.h"

#include <glm/mat4x4.hpp>

#include <functional>
#include <stack>

namespace edu::objects
{

class Object : public virtual IObject, public interface::Transformable
{
public:
    using TransformationType = glm::mat4;

    void enqueue();
    void dequeue();
    bool isQueued() const;

    const ColorType &color() const override;
    void setColor(const ColorType &color) override;

    void translate(const TranslationType &pos) override;
    void rotate(const RotationType &rot) override;
    void scale(const ScaleType &scale) override;

    // TODO: is this a good approach? Is this a good place?
    void composeTransformation(TransformationType &transform);

protected:
    ColorType m_color {};
    // If the object is queued, the renderer will update it
    bool m_queued {false};

private:
    std::stack<std::function<void(glm::mat4 &)>> m_compositionStack;
};

} // namespace edu::objects
