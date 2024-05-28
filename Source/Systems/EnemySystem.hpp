#pragma once

#include "Core/System.hpp"
#include "Core/Event/Event.hpp"

#define ROOM_CHANGE_TIMER 5.0f

class EnemySystem : public System
{
public:
    void Init();
    void Update(float dt);
    void Shutdown();

private:
    float roomChangeCd = 0.0f;
};