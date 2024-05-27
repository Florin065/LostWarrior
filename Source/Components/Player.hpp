#pragma once

#include <glm/glm.hpp>

#include "Core/Types.hpp"

struct Player
{
    Entity camera = MAX_ENTITIES + 1;
    glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
};

