#pragma once

#include <type_traits>

#include "glad/gl.h"


template<typename T>
constexpr GLenum glGetTypeEnum([[maybe_unused]] T x)
{
    if constexpr (std::is_same_v<T, int>) return GL_INT;
    if constexpr (std::is_same_v<T, unsigned int>) return GL_UNSIGNED_INT;
    if constexpr (std::is_same_v<T, float>) return GL_FLOAT;
    if constexpr (std::is_same_v<T, double>) return GL_DOUBLE;
}

