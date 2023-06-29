//
// Created by sfdd on 6/9/23.
//

#pragma once

#include "ILightSource.h"

#include <objects/Object.h>

namespace edu::objects::light
{

class LightSource : public virtual ILightSource, public Object
{
public:
    using LightProperetyType = glm::vec3;
    using PositionType = glm::vec3;
    using DirectionType = glm::vec3;

    struct LightMultipliers
    {
        LightProperetyType ambientMultiplier;
        LightProperetyType diffuseMultiplier;
        LightProperetyType specularMultiplier;
    };

    LightSource(const ColorType &color, const PositionType &initPosition, const LightMultipliers &lightMultipliers);
    ~LightSource() override = default;

    const LightProperetyType &ambient() const override;
    const LightProperetyType &diffuse() const override;
    const LightProperetyType &specular() const override;
    const PositionType &position() const override;
    void setColor(const ColorType &color) override;

    virtual void setPosition(const PositionType &position);

protected:
    void updateProperties();

    PositionType m_worldPosition;
    PositionType m_position;

    LightProperetyType m_ambient {};
    LightProperetyType m_diffuse {};
    LightProperetyType m_specular {};

    const LightMultipliers m_lightMultipliers;
};

} // namespace edu::objects::light
