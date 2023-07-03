//
// Created by sfdd on 6/10/23.
//

#include "Transformable.h"

namespace edu::objects::interface
{

auto Transformable::translation() const -> const TranslationType &
{
    return m_translation;
}

auto Transformable::rotation() const -> const RotationType &
{
    return m_rotation;
}

auto Transformable::scaling() const -> const ScaleType &
{
    return m_scale;
}

void Transformable::setTranslation(const TranslationType &pos)
{
    m_translation = pos;
}

void Transformable::setRotation(const RotationType &rot)
{
    m_rotation = rot;
}

void Transformable::setScale(const ScaleType &scale)
{
    m_scale = scale;
}

void Transformable::translate(const TranslationType &pos)
{
    setTranslation(m_translation + pos);
}

void Transformable::rotate(const RotationType &rot)
{
    setRotation(RotationType {rot.first, m_rotation.second + rot.second});
}

void Transformable::scale(const ScaleType &scale)
{
    setScale(m_scale * scale);
}

} // namespace edu::objects::interface
