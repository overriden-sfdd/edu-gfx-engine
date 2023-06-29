//
// Created by sfdd on 6/8/23.
//

#include "SpotLight.h"

namespace edu::objects::light
{

SpotLight::SpotLight(const ColorType &color, const PositionType &initPosition, const DirectionType &direction,
                     const LightMultipliers &lightMultipliers, const AttenuationConstants &attenuationConstants,
                     const IntensityConstants &intensityConstants)
    : PointLight(color, initPosition, lightMultipliers, attenuationConstants)
    , m_direction(direction)
    , m_intensityConstants(intensityConstants)
{
}

void SpotLight::setDirection(const DirectionType &direction)
{
    m_direction = direction;
}

auto SpotLight::intensity() -> IntensityConstants &
{
    return m_intensityConstants;
}

auto SpotLight::intensity() const -> const IntensityConstants &
{
    return m_intensityConstants;
}

auto SpotLight::direction() const -> const DirectionType &
{
    return m_direction;
}

} // namespace edu::objects::light