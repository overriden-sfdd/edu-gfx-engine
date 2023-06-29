//
// Created by sfdd on 6/11/23.
//

#include "Debugger.h"

#include <gfx/Material.h>
#include <objects/light/DirectLight.h>
#include <objects/light/PointLight.h>
#include <objects/light/SpotLight.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace edu::debug
{

namespace
{

constexpr std::pair<float, float> FromToMovePair {-10.f, 10.f};

std::string repeatedControlName(const std::string &name, size_t index)
{
    return name + "##" + std::to_string(index);
}

void setupLightControls(objects::light::LightSource *const light, size_t index)
{
    ImGui::SeparatorText("Color");
    auto color = light->color();
    ImGui::ColorEdit3(repeatedControlName("Color", index).c_str(), glm::value_ptr(color));
    light->setColor(color);
}

void setupPositionControls(objects::light::LightSource *const light, size_t index)
{
    auto position = light->position();
    ImGui::SliderFloat3(repeatedControlName("Position", index).c_str(), glm::value_ptr(position), FromToMovePair.first,
                        FromToMovePair.second);
    light->setPosition(position);
}

void setupDirectionControls(objects::light::DirectLight *const light, size_t index)
{
    ImGui::SeparatorText("Direction:");
    auto direction = light->direction();
    ImGui::SliderFloat3(repeatedControlName("Direction", index).c_str(), glm::value_ptr(direction),
                        FromToMovePair.first, FromToMovePair.second);
    light->setDirection(direction);
}

void setupAttenuation(objects::light::IPointLight *const light, size_t index)
{
    ImGui::SeparatorText("Attenuation:");
    auto &attenuation = light->attenuation();
    ImGui::SliderFloat(repeatedControlName("constant", index).c_str(), &attenuation.constant, 1e-6f, 5.f);
    ImGui::SliderFloat(repeatedControlName("linear", index).c_str(), &attenuation.linear, 0.f, 1.f);
    ImGui::SliderFloat(repeatedControlName("quadratic", index).c_str(), &attenuation.quadratic, 0.f, 1.f);
    // No need to set
}

void setupSpotLightIntensity(objects::light::SpotLight *const light, size_t index)
{
    ImGui::Text("Intensity [cos]");
    auto &intensity = light->intensity();
    const auto cutoffToAngle = [](const float cutoff) {
        return glm::degrees(std::acos(cutoff));
    };
    std::pair<float, float> cutOffs {cutoffToAngle(intensity.outerCutoffCos), cutoffToAngle(intensity.innerCutoffCos)};
    ImGui::SliderFloat(repeatedControlName("outer cutoff", index).c_str(), &cutOffs.first, -360.f, 360.f);
    ImGui::SliderFloat(repeatedControlName("inner cutoff", index).c_str(), &cutOffs.second, -360.f, 360.f);
    intensity.outerCutoffCos = std::cos(glm::radians(cutOffs.first));
    intensity.innerCutoffCos = std::cos(glm::radians(cutOffs.second));
    // No need to set
}

} // namespace

Debugger::Debugger(GLFWwindow **const window, const std::string &glslVersion)
    : m_glslVersion(glslVersion)
    , m_window(window)
{
    setupContext();
    setupControls();
    setupBackend();
    setupStyle();
}

void Debugger::setupContext()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = ImGui::GetIO();
}

void Debugger::setupControls()
{
    // Enable Keyboard Controls
    m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

void Debugger::setupStyle()
{
    ImGui::StyleColorsDark();
}

void Debugger::setupBackend()
{
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(*m_window, true);
    ImGui_ImplOpenGL3_Init(m_glslVersion.c_str());
}

void Debugger::setupDebugWindow()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code
    // to learn more about Dear ImGui!).
    if (m_windowFlags.showDemo) {
        ImGui::ShowDemoWindow(&m_windowFlags.showDemo);
    }

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static int counter = 0;

        ImGui::Begin("Object editor");

        ImGui::Text("Lighting section.");
        ImGui::Checkbox("Demo Window", &m_windowFlags.showDemo);

        if (ImGui::CollapsingHeader("Light")) {
            size_t index {0};

            if (ImGui::TreeNode("Direct light")) {
                for (const auto light : directLights) {
                    auto *const lightPtr = light.get();

                    setupLightControls(lightPtr, index);
                    setupPositionControls(lightPtr, index);
                    setupDirectionControls(lightPtr, index);

                    ++index;
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Point light")) {
                for (const auto light : pointLights) {
                    auto *const lightPtr = light.get();

                    setupLightControls(lightPtr, index);
                    setupPositionControls(lightPtr, index);
                    setupAttenuation(lightPtr, index);

                    ++index;
                }

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Spot light")) {
                for (const auto light : spotLights) {
                    auto *const lightPtr = light.get();

                    setupLightControls(lightPtr, index);
                    setupAttenuation(lightPtr, index);
                    setupSpotLightIntensity(lightPtr, index);

                    ++index;
                }
                ImGui::TreePop();
            }
        }

        ImGui::Text("Material section.");
        if (material) {
            ImGui::SeparatorText("Material:");
            ImGui::SliderFloat("shininess", &material->shininess, 0.f, 1024.f);
            ImGui::SliderFloat3("material specular", glm::value_ptr(material->specular), 0.f, 1.f);
        } else {
            ImGui::SeparatorText("No available material.");
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
        ImGui::End();
    }
}

void Debugger::processFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // if you want to also check for hovering use: ImGui::GetIO().WantCaptureMouse;
    m_mouseInUse = ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
}

void Debugger::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Debugger::clean()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Debugger::setWindowFlags(const WindowFlags windowFlags)
{
    m_windowFlags = windowFlags;
}

bool Debugger::isWindowFocused() const
{
    return m_mouseInUse;
}

} // namespace edu::debug
