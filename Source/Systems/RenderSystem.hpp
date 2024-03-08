#pragma once

#include "Core/System.hpp"


class RenderSystem : public System
{
public:
    void Init();
    void Shutdown();
    void Update(float dt);
};

