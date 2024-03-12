#pragma once


#include <glm/glm.hpp>


struct Vertex
{
    Vertex(glm::vec3 position,
           glm::vec3 color    = glm::vec3(1.0f),
           glm::vec3 normal   = glm::vec3(0.0f, 1.0f, 0.0f),
           glm::vec2 texCoord = glm::vec2(0.0f))
        : position(position)
        , color(color)
        , normal(normal)
        , texCoord(texCoord)
        {}

    Vertex& SetPosition(glm::vec3 position) { this->position = position; return *this; }
    Vertex& SetColor(glm::vec3 color) { this->color = color; return *this; }
    Vertex& SetNormal(glm::vec3 normal) { this->normal = normal; return *this; }
    Vertex& SetTexCoord(glm::vec3 texCoord) { this->texCoord = texCoord; return *this; }

    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

