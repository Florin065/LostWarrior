#pragma once

#include <memory>

#include "Core/Graphics/Mesh.hpp"
#include "Core/Graphics/Shader.hpp"

struct Renderable
{
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;
};

