//
// Created by sfdd on 5/29/23.
//

#pragma once

#include <glm/glm.hpp>

namespace edu::api
{

class Camera
{
public:
    struct EulerAngles
    {
        float yaw {-90.f};
        float pitch {0.f};
        float roll {0.f};
    };

    enum class CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UPWARD,
        DOWNWARD,
    };

    explicit Camera(float fov);
    Camera(const glm::vec3 &worldUp, const glm::vec3 &position, float fov);

    glm::mat4 lookAtMatrix() const;
    float fov() const;
    void setSpeedUp(bool speedUp);
    const glm::vec3 &cameraWorldPos() const;
    const glm::vec3 &cameraFront() const;

    void processMouseEvent(float xOffset, float yOffset, bool clampPitch = true);
    void processScrollEvent(float yOffset);
    void processKeyboardEvent(CameraMovement direction, float deltaTime);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

    // FoV should start at 45 degrees
    float m_fov {45.f};
    bool m_speedUp {false};
    EulerAngles m_eulerAngles {};

    glm::vec3 m_position {0.f, 0.f, 6.f};
    glm::vec3 m_worldUp {0.f, 1.f, 0.f};
    glm::vec3 m_front {0.f, 0.f, -1.f};
    glm::vec3 m_viewUp {};
    glm::vec3 m_viewRight {};
};

} // namespace edu::api
