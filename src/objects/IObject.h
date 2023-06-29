//
// Created by sfdd on 6/10/23.
//

#pragma once

#include <glm/vec3.hpp>

namespace edu::objects
{

class IObject
{
public:
    using ColorType = glm::vec3;

    ~IObject() = default;

    virtual const ColorType &color() const = 0;
    virtual void setColor(const ColorType &color) = 0;
};

} // namespace edu::objects
