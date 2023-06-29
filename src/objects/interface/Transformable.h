//
// Created by sfdd on 6/10/23.
//

#pragma once

#include "ITransformable.h"

namespace edu::objects::interface
{

class Transformable : public virtual ITransformable
{
public:
    const TranslationType &translation() const override;
    const RotationType &rotation() const override;
    const ScaleType &scaling() const override;

    void setTranslation(const TranslationType &pos) override;
    void setRotation(const RotationType &rot) override;
    void setScale(const ScaleType &scale) override;

    void translate(const TranslationType &pos) override;
    void rotate(const RotationType &rot) override;
    void scale(const ScaleType &scale) override;

protected:
    // These are glm types, so they have to be all zeros I guess... otherwise, there would've been an explicit zero
    // initialization, right?
    TranslationType m_translation {};
    RotationType m_rotation {};
    ScaleType m_scale {};
};

} // namespace edu::objects::interface
