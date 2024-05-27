#pragma once

#include "Core/System.hpp"
#include "Core/Event/Event.hpp"

class PlayerSystem : public System
{
public:
    void Init();
    void Update(float dt);
    void Shutdown();

private:
    float mDt;

    void KeyListener(Event const& event);
    void CursorListener(Event const& event);
};

