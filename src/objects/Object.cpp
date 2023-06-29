//
// Created by sfdd on 6/10/23.
//

#include "Object.h"

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
    setTranslation(pos);
    enqueue();
}

void Object::rotate(const RotationType &rot)
{
    setRotation(rot);
    enqueue();
}

void Object::scale(const ScaleType &scale)
{
    setScale(scale);
    enqueue();
}

} // namespace edu::objects