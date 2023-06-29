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
    virtual const glm::vec3 &ambient() const = 0;
    virtual const glm::vec3 &diffuse() const = 0;
    virtual const glm::vec3 &specular() const = 0;
    virtual const glm::vec3 &position() const = 0;
};

} // namespace edu::objects::light
