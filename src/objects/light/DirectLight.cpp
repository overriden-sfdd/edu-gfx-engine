//
// Created by sfdd on 6/8/23.
//

#include "DirectLight.h"

namespace edu::objects::light
{

DirectLight::DirectLight(const ColorType &color, const PositionType &initPosition, const DirectionType &lightDirection,
                         const LightMultipliers &lightMultipliers)
    : LightSource(color, initPosition, lightMultipliers)
    , m_direction(lightDirection)
{
}

auto DirectLight::direction() const -> const DirectionType &
{
    return m_direction;
}

void DirectLight::setDirection(const DirectionType &direction)
{
    m_direction = direction;
}

} // namespace edu::objects::light
