//
// Created by sfdd on 5/30/23.
//

#pragma once

#include <utility>

namespace edu::api
{

class Mouse
{
public:
    Mouse();
    Mouse(float initX, float initY);

    std::pair<float, float> updatePosition(float xPos, float yPos);

private:
    bool m_firstCallback {true};
    float m_lastX;
    float m_lastY;
};

} // namespace edu::api