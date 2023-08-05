//
// Created by sfdd on 6/9/23.
//

#pragma once

#include "ILightSource.h"

#include <objects/Object.h>
#include <objects/interface/IVisible.h>

namespace edu::objects::light
{

class LightSource : public Object, public ILightSource, public interface::IVisible
{
public:
    using LightProperetyType = glm::vec3;
    using DirectionType = glm::vec3;
    using ILightSource::PositionType;

    struct LightMultipliers
    {
        LightProperetyType ambientMultiplier;
        LightProperetyType diffuseMultiplier;
        LightProperetyType specularMultiplier;
    };

    LightSource(const ColorType &color, const PositionType &initPosition, const LightMultipliers &lightMultipliers);

    const LightProperetyType &ambient() const override;
    const LightProperetyType &diffuse() const override;
    const LightProperetyType &specular() const override;
    const ColorType &color() const override;
    const PositionType &position() const override;

    void setColor(const ColorType &color) override;
    void setPosition(const PositionType &position) override;

protected:
    void updateProperties();

    PositionType m_worldPosition;
    PositionType m_position;

    LightProperetyType m_ambient {};
    LightProperetyType m_diffuse {};
    LightProperetyType m_specular {};

    ColorType m_color;

    const LightMultipliers m_lightMultipliers;
};

} // namespace edu::objects::light
