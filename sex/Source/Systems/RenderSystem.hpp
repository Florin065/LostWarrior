#pragma once

#include "Core/System.hpp"
#include "Core/Event/Event.hpp"


class RenderSystem : public System
{
public:
    void Init();
    void Shutdown();
    void Update(float dt);
    void SwapCamera(Entity camera);

private:
    Entity mCameraEntity;
};

