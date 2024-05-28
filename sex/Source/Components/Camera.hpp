#pragma once

#include <glm/glm.hpp>

struct Camera
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 projection = glm::mat4(1.0f);
};

