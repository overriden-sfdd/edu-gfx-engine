//
// Created by sfdd on 5/30/23.
//

#include "Mouse.h"

#include <iostream>

namespace
{

constexpr float MouseSensitivity {0.1f};
// Assumes the screen is 800x600 and points to the center
constexpr float DefaultInitX {400.f};
constexpr float DefaultInitY {300.f};

} // namespace

namespace edu::api
{

Mouse::Mouse()
    : m_lastX {DefaultInitX}
    , m_lastY {DefaultInitY}
{
}

Mouse::Mouse(const float initX, const float initY)
    : m_lastX {initX}
    , m_lastY {initY}
{
}

std::pair<float, float> Mouse::updatePosition(const float xPos, const float yPos)
{
    if (m_firstCallback) {
        m_lastX = xPos;
        m_lastY = yPos;
        m_firstCallback = false;
    }

    const float xOffset {xPos - m_lastX};
    const float yOffset = {m_lastY - yPos};

    m_lastX = xPos;
    m_lastY = yPos;

    return {xOffset * MouseSensitivity, yOffset * MouseSensitivity};
}

} // namespace edu::api