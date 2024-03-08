#include "Systems/RenderSystem.hpp"

#include "Core/Coordinator.hpp"
extern Coordinator gCoordinator;

void RenderSystem::Init()
{

}

void RenderSystem::Shutdown()
{

}

void RenderSystem::Update(float dt)
{
    (void) dt;

    for (auto const& entity : mEntities)
    {
        (void) entity;
    }
}
