#pragma once

#include "Core/System.hpp"
#include "Core/Event/Event.hpp"

class CameraSystem : public System
{
public:
    void Init();
    void Update(float dt);
    void Shutdown();

private:
    float mDt;

    void FreecamKeyListener(Event const& event);
    void FreecamCursorListener(Event const& event);
};

