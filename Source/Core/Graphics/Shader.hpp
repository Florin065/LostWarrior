#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/gl.h"


class Shader
{
public:
    Shader(std::string const& vertPath,
           std::string const& fragPath,
           std::string const& geomPath = "");

    ~Shader()
    {
        glDeleteProgram(mId);
    }

    void Activate()
    {
        glUseProgram(mId);
    }

    template<typename T>
    constexpr
    void SetUniform(std::string const& name, T value) const
    {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, bool>)
        {
            glUniform1i(glGetUniformLocation(mId, name.c_str()), value);
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            glUniform1f(glGetUniformLocation(mId, name.c_str()), value);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            glUniform1d(glGetUniformLocation(mId, name.c_str()), value);
        }
        else if constexpr (std::is_same_v<T, glm::mat4>)
        {
            glUniformMatrix4fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
        }
    }

    template<glm::length_t L, typename T>
    constexpr
    void SetUniform(std::string const& name, glm::vec<L, T> value) const
    {
        if constexpr (std::is_same_v<T, int>)
        {
            switch (L)
            {
            case 1: return glUniform1iv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            case 2: return glUniform2iv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            case 3: return glUniform3iv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            case 4: return glUniform4iv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            }
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            switch (L)
            {
            case 1: return glUniform1fv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            case 2: return glUniform2fv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            case 3: return glUniform3fv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            case 4: return glUniform4fv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            }
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            switch (L)
            {
            case 1: return glUniform1dv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            case 2: return glUniform2dv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            case 3: return glUniform3dv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            case 4: return glUniform4dv(glGetUniformLocation(mId, name.c_str()), 1, glm::value_ptr(value));
            }
        }
    }

private:
    GLuint mId;
};

