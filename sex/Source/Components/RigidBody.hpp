#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>


struct RigidBody
{
    glm::vec3 velocity = glm::vec3(0.0f);
    std::unordered_map<std::string, glm::vec3> forces {};

    glm::vec3 angularVel = glm::vec3(0.0f);

    float mass = 1.0f;
    float drag = 0.0f;
};

