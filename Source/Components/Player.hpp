#pragma once

#include <glm/glm.hpp>

#include "Core/Types.hpp"

#define PLAYER_ARROW_CD 2.0f

struct Player
{
    Entity camera = MAX_ENTITIES + 1;
    glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
    float cooldown = 0.0f;
};

