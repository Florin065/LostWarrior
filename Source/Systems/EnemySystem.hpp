#pragma once

#include "Core/System.hpp"
#include "Core/Event/Event.hpp"

class EnemySystem : public System
{
public:
    void Init();
    void Update(float dt);
    void Shutdown();
};