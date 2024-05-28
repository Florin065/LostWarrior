#include "glad/gl.h"

#include "Core/Coordinator.hpp"

#include "Components/Renderable.hpp"
#include "Components/Transform.hpp"
#include "Components/Camera.hpp"

#include "Systems/RenderSystem.hpp"


extern Coordinator gCoordinator;


void RenderSystem::Init()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    {
        Transform cameraT;
        cameraT.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        Camera cameraC;
        mCameraEntity = gCoordinator.CreateEntity();
        gCoordinator.AddComponent(mCameraEntity, cameraT);
        gCoordinator.AddComponent(mCameraEntity, cameraC);
        gCoordinator.LogInfo("Initial camera created!");
    }
}

void RenderSystem::SwapCamera(Entity camera)
{
    gCoordinator.DestroyEntity(mCameraEntity);
    mCameraEntity = camera;
}

void RenderSystem::Shutdown()
{

}

void RenderSystem::Update([[maybe_unused]] float dt)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto& cameraC = gCoordinator.GetComponent<Camera>(mCameraEntity);

    for (auto const& entity : mEntities)
    {
        auto& transform  = gCoordinator.GetComponent<Transform>(entity);
        auto& renderable = gCoordinator.GetComponent<Renderable>(entity);

        renderable.shader->Activate();
        renderable.shader->SetUniform("Model", transform.model);
        renderable.shader->SetUniform("View", cameraC.view);
        renderable.shader->SetUniform("Projection", cameraC.projection);
        renderable.model->Draw(renderable.shader);
    }
}

