//
// Created by sfdd on 6/16/23.
//

#pragma once

#include <glm/vec3.hpp>

namespace edu::gfx
{

struct Material
{
    glm::vec3 specular {};
    float shininess {0.f};
};

} // namespace edu::gfx
