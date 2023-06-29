//
// Created by sfdd on 6/10/23.
//

#pragma once

#include <glm/vec3.hpp>

#include <utility>

namespace edu::objects::interface
{

class ITransformable
{
public:
    using TranslationType = glm::vec3;
    // Axis <-> angle pair
    using RotationType = std::pair<glm::vec3, float>;
    using ScaleType = glm::vec3;

    virtual ~ITransformable() = default;

    virtual const TranslationType &translation() const = 0;
    virtual const RotationType &rotation() const = 0;
    virtual const ScaleType &scaling() const = 0;

    virtual void setTranslation(const TranslationType &pos) = 0;
    virtual void setRotation(const RotationType &rot) = 0;
    virtual void setScale(const ScaleType &scale) = 0;

    virtual void translate(const TranslationType &pos) = 0;
    virtual void rotate(const RotationType &rot) = 0;
    virtual void scale(const ScaleType &scale) = 0;
};

} // namespace edu::objects::interface
