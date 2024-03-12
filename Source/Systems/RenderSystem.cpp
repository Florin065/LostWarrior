#include "glad/gl.h"

#include "Components/Renderable.hpp"

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

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto const& entity : mEntities)
    {
        auto renderable = gCoordinator.GetComponent<Renderable>(entity);

        renderable.shader->Activate();
        renderable.mesh->Render();
    }
}

