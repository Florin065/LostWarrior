#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 __pos = glm::vec3(0.0f);
    glm::vec3 __rot = glm::vec3(0.0f);
    glm::vec3 __scl = glm::vec3(1.0f);

    Transform(glm::vec3 pos = glm::vec3(0.0f),
            glm::vec3 rot = glm::vec3(0.0f),
            glm::vec3 scl = glm::vec3(1.0f))
        : __pos(pos)
        , __rot(rot)
        , __scl(scl)
    {
        __UpdateModel();
    }

    void Rotate(glm::vec3 rot)
    {
        __rot += rot; __UpdateModel();
    }

    void Translate(glm::vec3 pos)
    {
        __pos += pos; __UpdateModel();
    }

    void Scale(glm::vec3 scl)
    {
        __scl += scl; __UpdateModel();
    }

    void SetRotation(glm::vec3 rot)
    {
        __rot = rot; __UpdateModel();
    }

    void SetPosition(glm::vec3 pos)
    {
        __pos = pos; __UpdateModel();
    }

    void SetScale(glm::vec3 scl)
    {
        __scl = scl; __UpdateModel();
    }

    void __UpdateModel()
    {
        const glm::mat4 t = translate(glm::mat4(1.0f), __pos);
        const glm::mat4 r = mat4_cast(glm::quat(__rot));
        const glm::mat4 s = scale(glm::mat4(1.0f), __scl);

        model = t * r * s;
    }
};

