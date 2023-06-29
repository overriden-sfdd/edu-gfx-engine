//
// Created by sfdd on 6/10/23.
//

#pragma once

#include "IObject.h"
#include "interface/Transformable.h"

namespace edu::objects
{

class Object : public virtual IObject, public interface::Transformable
{
public:
    void enqueue();
    void dequeue();
    bool isQueued() const;

    const ColorType &color() const override;
    void setColor(const ColorType &color) override;

    void translate(const TranslationType &pos) override;
    void rotate(const RotationType &rot) override;
    void scale(const ScaleType &scale) override;

protected:
    ColorType m_color {};
    // If the object is queued, the renderer will update it
    bool m_queued {false};
};

} // namespace edu::objects
