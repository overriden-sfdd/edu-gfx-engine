//
// Created by sfdd on 6/10/23.
//

#include "Object.h"
#include "Asset.h"

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

void Object::translate(const TranslationType &pos)
{
    interface::Transformable::translate(pos);
    m_compositionStack.emplace([this](glm::mat4 &transform) {
        transform = glm::translate(transform, m_translation);
    });
    enqueue();
}

void Object::rotate(const RotationType &rot)
{
    interface::Transformable::rotate(rot);
    m_compositionStack.emplace([this](glm::mat4 &transform) {
        transform = glm::rotate(transform, m_rotation.second, m_rotation.first);
    });
    enqueue();
}

void Object::scale(const ScaleType &scaling)
{
    interface::Transformable::scale(scaling);
    m_compositionStack.emplace([this](glm::mat4 &transform) {
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

const Asset *Object::asset() const
{
    return m_asset.get();
}

void Object::setAsset(const std::shared_ptr<Asset> asset)
{
    m_asset = asset;
}

} // namespace edu::objects