//
// Created by sfdd on 6/11/23.
//

#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <memory>
#include <string>
#include <vector>

namespace edu::objects::light
{
class DirectLight;
class PointLight;
class SpotLight;
} // namespace edu::objects::light

namespace edu::gfx
{
struct Material;
} // namespace edu::gfx

namespace edu::debug
{

class Debugger
{
public:
    template <class LightType>
    using Container = std::vector<std::shared_ptr<LightType>>;

    using DirectLightContainer = Container<objects::light::DirectLight>;
    using PointLightContainer = Container<objects::light::PointLight>;
    using SpotLightContainer = Container<objects::light::SpotLight>;

    struct WindowFlags
    {
        bool showDemo {false};
    };

    Debugger(GLFWwindow **window, const std::string &glslVersion);

    void setupContext();
    void setupControls();
    void setupStyle();
    void setupBackend();
    void setupDebugWindow();

    void processFrame();
    void render();
    void clean();
    void setWindowFlags(WindowFlags windowFlags);
    bool isWindowFocused() const;

    DirectLightContainer directLights;
    PointLightContainer pointLights;
    SpotLightContainer spotLights;
    // Shouldn't be passed like that, but will do for now
    std::shared_ptr<gfx::Material> material;

private:
    const std::string m_glslVersion;
    bool m_mouseInUse {false};

    ImVec4 m_clearColor {0.45f, 0.55f, 0.60f, 1.00f};
    ImGuiIO m_io;
    GLFWwindow **const m_window;
    WindowFlags m_windowFlags;
};

} // namespace edu::debug
