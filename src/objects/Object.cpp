//
// Created by sfdd on 6/10/23.
//

#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

namespace edu::objects
{

void Object::enqueue()
{
    m_queued = true;
}

void Object::dequeue()
{
    m_queued = false;
}

bool Object::isQueued() const
{
    return m_queued;
}

auto Object::color() const -> const ColorType &
{
    return m_color;
}

void Object::setColor(const ColorType &color)
{
    m_color = color;
}

void Object::translate(const TranslationType &pos)
{
    interface::Transformable::translate(pos);
    m_compositionStack.push([this](glm::mat4 &transform) {
        transform = glm::translate(transform, m_translation);
    });
    enqueue();
}

void Object::rotate(const RotationType &rot)
{
    interface::Transformable::rotate(rot);
    m_compositionStack.push([this](glm::mat4 &transform) {
        transform = glm::rotate(transform, m_rotation.second, m_rotation.first);
    });
    enqueue();
}

void Object::scale(const ScaleType &scale)
{
    interface::Transformable::scale(scale);
    m_compositionStack.push([this](glm::mat4 &transform) {
        transform = glm::scale(transform, m_scale);
    });
    enqueue();
}

void Object::composeTransformation(TransformationType &transform)
{
    for (; !m_compositionStack.empty();) {
        m_compositionStack.top()(transform);
        m_compositionStack.pop();
    }
}

} // namespace edu::objects