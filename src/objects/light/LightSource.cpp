//
// Created by sfdd on 6/9/23.
//

#include "LightSource.h"

#include <glm/glm.hpp>

namespace edu::objects::light
{

LightSource::LightSource(const ColorType &color, const PositionType &initPosition,
                         const LightMultipliers &lightMultipliers)
    : m_worldPosition(initPosition)
    , m_position(initPosition)
    , m_lightMultipliers(lightMultipliers)
{
    setColor(color);
}

auto LightSource::ambient() const -> const LightProperetyType &
{
    return m_ambient;
}

auto LightSource::diffuse() const -> const LightProperetyType &
{
    return m_diffuse;
}

auto LightSource::specular() const -> const LightProperetyType &
{
    return m_specular;
}

auto LightSource::position() const -> const PositionType &
{
    return m_position;
}

void LightSource::setColor(const ColorType &color)
{
    if (glm::any(glm::notEqual(color, m_color))) {
        m_color = color;
        updateProperties();
    }
}

void LightSource::setPosition(const PositionType &position)
{
    m_position = position;
}

void LightSource::updateProperties()
{
    m_ambient = m_color * m_lightMultipliers.ambientMultiplier;
    m_diffuse = m_color * m_lightMultipliers.diffuseMultiplier;
    m_specular = m_color * m_lightMultipliers.specularMultiplier;
}

} // namespace edu::objects::light