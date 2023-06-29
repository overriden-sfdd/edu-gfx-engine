//
// Created by sfdd on 6/8/23.
//

#include "PointLight.h"

namespace edu::objects::light
{

PointLight::PointLight(const ColorType &color, const PositionType &initPosition,
                       const LightMultipliers &lightMultipliers, const AttenuationConstants &attenuationConstants)
    : LightSource(color, initPosition, lightMultipliers)
    , m_attenuationConstants(attenuationConstants)
{
}

auto PointLight::attenuation() const -> const AttenuationConstants &
{
    return m_attenuationConstants;
}

auto PointLight::attenuation() -> AttenuationConstants &
{
    return m_attenuationConstants;
}

} // namespace edu::objects::light