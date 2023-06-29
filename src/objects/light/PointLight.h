//
// Created by sfdd on 6/8/23.
//

#pragma once

#include "IPointLight.h"
#include "LightSource.h"

namespace edu::objects::light
{

class PointLight : public LightSource, public IPointLight
{
public:
    PointLight(const ColorType &color, const PositionType &initPosition, const LightMultipliers &lightMultipliers,
               const AttenuationConstants &attenuationConstants);

    const AttenuationConstants &attenuation() const override;
    AttenuationConstants &attenuation() override;

private:
    AttenuationConstants m_attenuationConstants;
};

} // namespace edu::objects::light
