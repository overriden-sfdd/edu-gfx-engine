//
// Created by sfdd on 6/9/23.
//

#pragma once

#include <glm/vec3.hpp>

namespace edu::objects::light
{

class ILightSource
{
public:
    using PositionType = glm::vec3;

    virtual ~ILightSource() = default;

    virtual const glm::vec3 &ambient() const = 0;
    virtual const glm::vec3 &diffuse() const = 0;
    virtual const glm::vec3 &specular() const = 0;
    virtual const glm::vec3 &position() const = 0;
    virtual void setPosition(const PositionType &position) = 0;
};

} // namespace edu::objects::light
