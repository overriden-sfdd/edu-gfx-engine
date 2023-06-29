//
// Created by sfdd on 5/20/23.
//

#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

namespace
{

constexpr auto CheckStatus = [](const auto &ivFunc, const auto &infoLogFunc, const auto &errorMsg) {
    int32_t success {0};
    const auto infoLogSize {512};
    char infoLog[infoLogSize] {};
    ivFunc(&success);
    if (!success) {
        infoLogFunc(infoLogSize, infoLog);
        std::cout << errorMsg << infoLog << std::endl;
    }
};

} // namespace

namespace edu::gfx
{

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
    : m_shaderProgramId {glCreateProgram()}
{
    // Retrieve the shader source code from the files
    std::ifstream vShaderFile {};
    std::ifstream fShaderFile {};
    // Ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream {};
        std::stringstream fShaderStream {};
        // Read files' buffer contents into the corresponding streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        m_vertexSource = vShaderStream.str();
        m_fragmentSource = fShaderStream.str();
    } catch (const std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    processShaders();
}

uint32_t Shader::programId() const
{
    return m_shaderProgramId;
}

void Shader::useShaderProgram() const
{
    glUseProgram(m_shaderProgramId);
}

std::pair<uint32_t, uint32_t> Shader::compileShaders() const
{
    // Create vertex shader and compile on runtime
    const auto rawVertexData = m_vertexSource.c_str();
    const auto vertexShader {glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertexShader, 1, &rawVertexData, nullptr);
    glCompileShader(vertexShader);
    CheckStatus(
        [vertexShader](int32_t *success) {
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, success);
        },
        [vertexShader](const auto infoLogSize, auto &infoLog) {
            glGetShaderInfoLog(vertexShader, infoLogSize, nullptr, infoLog);
        },
        "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");

    // Create fragment shader and compile on runtime
    const auto rawFragmentData = m_fragmentSource.c_str();
    const auto fragmentShader {glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader, 1, &rawFragmentData, nullptr);
    glCompileShader(fragmentShader);
    CheckStatus(
        [fragmentShader](int32_t *success) {
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, success);
        },
        [fragmentShader](const auto infoLogSize, auto &infoLog) {
            glGetShaderInfoLog(fragmentShader, infoLogSize, nullptr, infoLog);
        },
        "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    return {vertexShader, fragmentShader};
}

void Shader::linkShaders(const uint32_t vertexShader, const uint32_t fragmentShader) const
{
    glAttachShader(m_shaderProgramId, vertexShader);
    glAttachShader(m_shaderProgramId, fragmentShader);
    glLinkProgram(m_shaderProgramId);
    CheckStatus(
        [this](int32_t *success) {
            glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, success);
        },
        [this](const auto infoLogSize, auto &infoLog) {
            glGetProgramInfoLog(m_shaderProgramId, infoLogSize, nullptr, infoLog);
        },
        "ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
}

void Shader::processShaders() const
{
    const auto [vertexShader, fragmentShader] = compileShaders();
    linkShaders(vertexShader, fragmentShader);
    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::setValue(const std::string &name, const int32_t value) const
{
    const auto uniformLocation = findUniformLocation(name);
    if (uniformLocation != -1) {
        glUniform1i(uniformLocation, value);
    }
}

void Shader::setValue(const std::string &name, const bool value) const
{
    const auto uniformLocation = findUniformLocation(name);
    if (uniformLocation != -1) {
        glUniform1i(uniformLocation, value);
    }
}

void Shader::setValue(const std::string &name, const float value) const
{
    const auto uniformLocation = findUniformLocation(name);
    if (uniformLocation != -1) {
        glUniform1f(uniformLocation, value);
    }
}

void Shader::setValue(const std::string &name, const glm::mat4 &value) const
{
    const auto uniformLocation = findUniformLocation(name);
    if (uniformLocation != -1) {
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::setValue(const std::string &name, const glm::vec3 &value) const
{
    const auto uniformLocation = findUniformLocation(name);
    if (uniformLocation != -1) {
        glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
    }
}

int32_t Shader::findUniformLocation(const std::string &name) const
{
    const auto uniformLocation = glGetUniformLocation(m_shaderProgramId, name.c_str());
    if (uniformLocation == -1) {
        std::cout << "Location for a uniform with name " << name << " was not found! No value was set." << std::endl;
    }
    return uniformLocation;
}

} // namespace edu::gfx