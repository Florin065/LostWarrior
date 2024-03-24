#pragma once

#include <glm/glm.hpp>

struct RigidBody
{
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 force = glm::vec3(0.0f);

    glm::vec3 angularVel = glm::vec3(0.0f);

    float     mass = 1.0f;
    glm::vec3 drag = glm::vec3(0.0f);
};
