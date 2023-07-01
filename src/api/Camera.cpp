//
// Created by sfdd on 5/29/23.
//

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <utility>

namespace
{

constexpr auto CameraMovementSpeed {3.f};
constexpr auto CameraSpeedMultiplier {5.f};
// Clamp ranges
constexpr std::pair<float, float> PitchRange {-89.f, 89.f};
constexpr std::pair<float, float> FovRange {1.f, 45.f};

} // namespace

namespace edu::api
{

Camera::Camera(const float fov)
    : m_fov {fov}
{
    updateCameraVectors();
}

Camera::Camera(const glm::vec3 &worldUp, const glm::vec3 &position, const float fov)
    : m_fov {fov}
    , m_worldUp(worldUp)
    , m_position(position)
{
    updateCameraVectors();
}

glm::mat4 Camera::lookAtMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_viewUp);
}

float Camera::fov() const
{
    return m_fov;
}

void Camera::setSpeedUp(bool speedUp)
{
    m_speedUp = speedUp;
}

const glm::vec3 &Camera::cameraWorldPos() const
{
    return m_position;
}

const glm::vec3 &Camera::cameraFront() const
{
    return m_front;
}

void Camera::processMouseEvent(float xOffset, float yOffset, const bool clampPitch)
{
    m_eulerAngles.yaw += xOffset;
    m_eulerAngles.pitch += yOffset;
    if (clampPitch) {
        m_eulerAngles.pitch = std::clamp(m_eulerAngles.pitch, PitchRange.first, PitchRange.second);
    } else {
        m_eulerAngles.pitch = glm::mod(m_eulerAngles.pitch, 360.0f);
    }
    m_eulerAngles.yaw = glm::mod(m_eulerAngles.yaw, 360.0f);

    updateCameraVectors();
}

void Camera::processScrollEvent(const float yOffset)
{
    m_fov -= yOffset;
    m_fov = std::clamp(m_fov, FovRange.first, FovRange.second);
}

void Camera::processKeyboardEvent(const Camera::CameraMovement direction, const float deltaTime)
{
    const float velocity = CameraMovementSpeed * deltaTime * (m_speedUp ? CameraSpeedMultiplier : 1.f);
    switch (direction) {
    case CameraMovement::FORWARD:
        m_position += m_front * velocity;
        break;
    case CameraMovement::BACKWARD:
        m_position -= m_front * velocity;
        break;
    case CameraMovement::RIGHT:
        m_position += m_viewRight * velocity;
        break;
    case CameraMovement::LEFT:
        m_position -= m_viewRight * velocity;
        break;
    case CameraMovement::UPWARD:
        m_position += m_viewUp * velocity;
        break;
    case CameraMovement::DOWNWARD:
        m_position -= m_viewUp * velocity;
        break;
    }
}

void Camera::updateCameraVectors()
{
    // Calculate the new front vector
    const auto xDirection = std::cos(glm::radians(m_eulerAngles.yaw)) * std::cos(glm::radians(m_eulerAngles.pitch));
    const auto yDirection = std::sin(glm::radians(m_eulerAngles.pitch));
    const auto zDirection = std::sin(glm::radians(m_eulerAngles.yaw)) * std::cos(glm::radians(m_eulerAngles.pitch));
    m_front = glm::normalize(glm::vec3 {xDirection, yDirection, zDirection});
    // Re-calculate the right and up camera vectors and normalize:
    // the closer their lengths to 0 the more you look up or down which results in slower movement
    m_viewRight = glm::normalize(glm::cross(m_front, m_worldUp));
    m_viewUp = glm::normalize(glm::cross(m_viewRight, m_front));
}

} // namespace edu::api
