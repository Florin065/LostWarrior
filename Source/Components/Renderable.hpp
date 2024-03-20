#pragma once

#include <memory>

#include "Core/Graphics/Model.hpp"
#include "Core/Graphics/Shader.hpp"

struct Renderable
{
    std::shared_ptr<Model> model;
    std::shared_ptr<Shader> shader;
};

