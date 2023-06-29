//
// Created by sfdd on 5/20/23.
//

#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <string>

namespace edu::gfx
{

class Shader
{
public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    uint32_t programId() const;
    void useShaderProgram() const;
    void processShaders() const;

    /* Utility uniform functions */
    void setValue(const std::string &name, int32_t value) const;
    void setValue(const std::string &name, bool value) const;
    void setValue(const std::string &name, float value) const;
    void setValue(const std::string &name, const glm::mat4 &value) const;
    void setValue(const std::string &name, const glm::vec3 &value) const;

private:
    /**
     * @returns {vertexShader, fragmentShader} id pair
     */
    std::pair<uint32_t, uint32_t> compileShaders() const;
    void linkShaders(uint32_t vertexShader, uint32_t fragmentShader) const;
    int32_t findUniformLocation(const std::string &name) const;

    // The program ID
    uint32_t m_shaderProgramId {0};
    std::string m_vertexSource;
    std::string m_fragmentSource;
};

} // namespace edu::gfx
