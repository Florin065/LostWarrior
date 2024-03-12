#pragma once

#include <memory>

#include "Core/Graphics/Mesh.hpp"


struct Renderable
{
    std::shared_ptr<Mesh> mesh;
};

