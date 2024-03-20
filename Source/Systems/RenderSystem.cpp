#include "glad/gl.h"

#include "Components/Renderable.hpp"
#include "Components/Transform.hpp"
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
    mDt = dt;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto const& entity : mEntities)
    {
        auto& transform  = gCoordinator.GetComponent<Transform>(entity);
        auto& renderable = gCoordinator.GetComponent<Renderable>(entity);

        renderable.shader->Activate();
        renderable.shader->SetUniform("Model", transform.model);
        renderable.model->Draw(renderable.shader);
    }
}

