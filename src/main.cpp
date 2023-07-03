// GLAD headers must be before the GLFW's
#include <glad/glad.h>

#include "api/Camera.h"
#include "api/Mouse.h"
#include "debug/Debugger.h"
#include "gfx/Asset.h"
#include "gfx/AssetModel.h"
#include "gfx/Material.h"
#include "gfx/Shader.h"
#include "objects/light/DirectLight.h"
#include "objects/light/PointLight.h"
#include "objects/light/SpotLight.h"
#include "renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include <iostream>

namespace
{

constexpr int32_t WindowWidth {800};
constexpr int32_t WindowHeight {600};

const std::vector<glm::vec3> PointLightPositions {
    glm::vec3 {0.7f, 0.2f, 2.0f},
    glm::vec3 {2.3f, -3.3f, -4.0f},
    glm::vec3 {-4.0f, 2.0f, -12.0f},
    glm::vec3 {0.0f, 0.0f, -3.0f},
};

// Global mouse init
edu::api::Mouse mouse {};
// Global camera init
edu::api::Camera camera {45.f};

bool MouseAvailable {true};
float DeltaTime {0.f};
float MixValue {0.f};

} // namespace

bool isKeyPressed(GLFWwindow *const window, const int32_t key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool isKeyReleased(GLFWwindow *const window, const int32_t key)
{
    return glfwGetKey(window, key) == GLFW_RELEASE;
}

void pollKeyboard(GLFWwindow *const *const window)
{
    // General
    if (isKeyPressed(*window, GLFW_KEY_LEFT_SHIFT)) {
        camera.setSpeedUp(true);
    }
    if (isKeyReleased(*window, GLFW_KEY_LEFT_SHIFT)) {
        camera.setSpeedUp(false);
    }
    if (isKeyPressed(*window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(*window, true);
    }
    if (isKeyPressed(*window, GLFW_KEY_LEFT_CONTROL)) {
        glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    // Map camera movement
    using Direction = edu::api::Camera::CameraMovement;
    if (isKeyPressed(*window, GLFW_KEY_W)) {
        camera.processKeyboardEvent(Direction::FORWARD, DeltaTime);
    }
    if (isKeyPressed(*window, GLFW_KEY_S)) {
        camera.processKeyboardEvent(Direction::BACKWARD, DeltaTime);
    }
    if (isKeyPressed(*window, GLFW_KEY_D)) {
        camera.processKeyboardEvent(Direction::RIGHT, DeltaTime);
    }
    if (isKeyPressed(*window, GLFW_KEY_A)) {
        camera.processKeyboardEvent(Direction::LEFT, DeltaTime);
    }
    if (isKeyPressed(*window, GLFW_KEY_Q)) {
        camera.processKeyboardEvent(Direction::UPWARD, DeltaTime);
    }
    if (isKeyPressed(*window, GLFW_KEY_X)) {
        camera.processKeyboardEvent(Direction::DOWNWARD, DeltaTime);
    }

    // Map mix value
    if (isKeyPressed(*window, GLFW_KEY_UP)) {
        MixValue += 0.005f;
    }
    if (isKeyPressed(*window, GLFW_KEY_DOWN)) {
        MixValue -= 0.005f;
    }
    MixValue = std::clamp(MixValue, 0.f, 1.f);
}

void framebuffer_size_callback(GLFWwindow * /*window*/, const int32_t width, const int32_t height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, const double xPos, const double yPos)
{
    const bool cursorShown = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
    if (!cursorShown) {
        const auto [xOffset, yOffset] = mouse.updatePosition(static_cast<float>(xPos), static_cast<float>(yPos));
        camera.processMouseEvent(xOffset, yOffset);
    }
}

void scroll_callback(GLFWwindow * /*window*/, const double /*xOffset*/, const double yOffset)
{
    camera.processScrollEvent(static_cast<float>(yOffset));
}

void setupWindow(GLFWwindow **const window)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    *window = glfwCreateWindow(WindowWidth, WindowHeight, "LearnOpenGL", nullptr, nullptr);
    if (*window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(*window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, WindowWidth, WindowHeight);
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
    glfwSetCursorPosCallback(*window, mouse_callback);
    glfwSetScrollCallback(*window, scroll_callback);
}

int main()
{
    glfwInit();
    GLFWwindow *window {nullptr};
    setupWindow(&window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // Enable z-buffer for depth testing
    glEnable(GL_DEPTH_TEST);

    // Setup ImGui debugger
    edu::debug::Debugger debugger(&window, "#version 130");

    edu::renderer::Renderer renderer(std::make_unique<edu::gfx::AssetModel>());

    const auto fancyBox = std::make_shared<edu::gfx::Material>(glm::vec3 {0.5f, 0.5f, 0.5f}, 64.f);

    edu::debug::Debugger::DirectLightContainer directLightPointers;
    edu::debug::Debugger::PointLightContainer pointLightPointers;
    edu::debug::Debugger::SpotLightContainer spotLightPointers;
    {
        using namespace edu::objects::light;

        const glm::vec3 color {1.f, 1.f, 1.f};
        const glm::vec3 worldPosition {1.2f, 8.0f, -4.0f};
        const glm::vec3 direction {-0.2f, -1.0f, -0.3f};

        const LightSource::LightMultipliers lightMultipliers {
            .ambientMultiplier = {0.1f, 0.1f, 0.1f},
            .diffuseMultiplier = {0.9f, 0.9f, 0.9f},
            .specularMultiplier = {1.0f, 1.0f, 1.0f},
        };

        directLightPointers.push_back(
            std::make_shared<DirectLight>(glm::vec3 {.1f, .3f, 1.0f}, worldPosition, direction, lightMultipliers));

        // Point light for a 50m distance based on
        // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
        const SpotLight::AttenuationConstants attenuationConstants {
            .constant = 1.f,
            .linear = 0.1f,
            .quadratic = 0.04f,
        };

        for (auto position : PointLightPositions) {
            pointLightPointers.push_back(
                std::make_shared<PointLight>(color, position, lightMultipliers, attenuationConstants));
        }

        const SpotLight::IntensityConstants intensityConstants {
            .innerCutoffCos = std::cos(glm::radians(12.5f)),
            .outerCutoffCos = std::cos(glm::radians(17.5f)),
        };
        spotLightPointers.push_back(std::make_shared<SpotLight>(color, worldPosition, direction, lightMultipliers,
                                                                attenuationConstants, intensityConstants));
    }

    debugger.directLights = directLightPointers;
    debugger.pointLights = pointLightPointers;
    debugger.spotLights = spotLightPointers;
    debugger.material = fancyBox;

    // Time between current frame and last frame
    float lastFrame {0.f};
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        debugger.processFrame();
        debugger.setupDebugWindow();
        // Mouse is unavailable while it's focused by the debugger
        if (!MouseAvailable && !debugger.isWindowFocused()) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        MouseAvailable = !debugger.isWindowFocused();

        const float currentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Rendering commands
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Poll keyboard
        pollKeyboard(&window);

        auto backpack = renderer.setCurrentAsset(edu::gfx::Mapping::AssetId::Backpack);
        (void)backpack;

        glm::mat4 model = glm::mat4(1.0f);
        renderer.setModelMatrix(model);

        // Calculate the coordinate system matrices and send to the shaders
        const auto &view = camera.lookAtMatrix();
        const auto projection
            = glm::perspective(glm::radians(camera.fov()), WindowWidth / static_cast<float>(WindowHeight), 0.1f, 100.f);
        renderer.setViewMatrix(view);
        renderer.setPerspectiveMatrix(projection);

        renderer.onRenderStep();
        renderer.draw();

        auto hangingLight = renderer.setCurrentAsset(edu::gfx::Mapping::AssetId::HangingLight);
        hangingLight->enqueue();

        // TODO: store a map of the asset positions?..
        renderer.setModelMatrix(glm::translate(model, {0.f, 12.f, 0.f}));
        renderer.onRenderStep();
        renderer.draw();

        debugger.render();

        // Buffers and general utility
        glfwSwapBuffers(window);
    }

    // Clean up
    debugger.clean();
    renderer.clean();
    glfwTerminate();

    return 0;
}
