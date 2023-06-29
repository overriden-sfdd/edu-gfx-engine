//
// Created by sfdd on 6/8/23.
//

#pragma once

#include "PointLight.h"

namespace edu::objects::light
{

class SpotLight : public PointLight
{
public:
    struct IntensityConstants
    {
        float innerCutoffCos;
        float outerCutoffCos;
    };

    SpotLight(const ColorType &color, const PositionType &initPosition, const DirectionType &direction,
              const LightMultipliers &lightMultipliers, const AttenuationConstants &attenuationConstants,
              const IntensityConstants &intensityConstants);

    IntensityConstants &intensity();
    void setDirection(const DirectionType &direction);

    const IntensityConstants &intensity() const;
    const DirectionType &direction() const;

private:
    DirectionType m_direction;

    IntensityConstants m_intensityConstants;
};

} // namespace edu::objects::light
