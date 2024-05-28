#pragma once

#include "Core/System.hpp"
#include "Core/Event/Event.hpp"


class PhysicsSystem : public System
{
public:
    void Init();
    void Shutdown();
    void Update(float dt);

private:
};

