//
// Created by sfdd on 6/8/23.
//

#pragma once

#include "LightSource.h"

namespace edu::objects::light
{

class DirectLight : public LightSource
{
public:
    DirectLight(const ColorType &color, const PositionType &initPosition, const DirectionType &lightDirection,
                const LightMultipliers &lightMultipliers);

    const DirectionType &direction() const;
    void setDirection(const DirectionType &direction);

private:
    DirectionType m_direction;
};

} // namespace edu::objects::light
