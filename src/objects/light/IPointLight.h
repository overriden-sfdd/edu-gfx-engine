//
// Created by sfdd on 6/9/23.
//

#pragma once

namespace edu::objects::light
{

class IPointLight
{
public:
    struct AttenuationConstants
    {
        float constant;
        float linear;
        float quadratic;
    };

    virtual ~IPointLight() = default;

    virtual const AttenuationConstants &attenuation() const = 0;
    virtual AttenuationConstants &attenuation() = 0;
};

} // namespace edu::objects::light
