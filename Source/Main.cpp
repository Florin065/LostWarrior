#include <chrono>
#include <glm/glm.hpp>
#include <random>

#include "Core/Coordinator.hpp"
#include "Core/Types.hpp"
#include "Core/ResourceManager.hpp"

#include "Components/Transform.hpp"
#include "Components/Renderable.hpp"
#include "Components/Camera.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Collider.hpp"

#include "Systems/CollisionSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/MapGenerationSystem.hpp"

#define GLAD_GL_IMPLEMENTATION
#include "Core/Window/WindowManager.hpp"

Coordinator gCoordinator(LogLevel::NORMAL);
ResourceManager gResourceManager;

int dimension = 1;
std::vector<Entity> entities;

static auto sWindowManager = std::make_unique<WindowManager>(1920, 1080, "Game");


void listenerKey(Event const& event)
{
    if (event.GetParam<int>(Events::Input::Async::Key::KEY) == GLFW_KEY_ESCAPE)
    {
        sWindowManager->SetWindowShouldClose();
    }
}


int main(void)
{

    gCoordinator.AddListener(FUNCTION_LISTENER(Events::Input::Async::Key::ID,
                                               listenerKey));

    gCoordinator.RegisterComponent<Transform>();
    gCoordinator.RegisterComponent<Renderable>();
    gCoordinator.RegisterComponent<Camera>();
    gCoordinator.RegisterComponent<RigidBody>();
    gCoordinator.RegisterComponent<Collider>();

    auto cameraSystem = gCoordinator.RegisterSystem<CameraSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<Camera>());
        gCoordinator.SetSystemSignature<CameraSystem>(signature);
    }
    cameraSystem->Init();

    /* Render system must be initialized AFTER the camera system, as its
     * `Init()` method creates an entity with a `Transform` and `Camera`
     * components, and that entity must be in the `CameraSystem`s set of
     * entities. */
    auto renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<Renderable>());
        gCoordinator.SetSystemSignature<RenderSystem>(signature);
    }
    renderSystem->Init();

    auto physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<RigidBody>());
        gCoordinator.SetSystemSignature<PhysicsSystem>(signature);
    }
    physicsSystem->Init();

    auto collisionSystem = gCoordinator.RegisterSystem<CollisionSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<Collider>());
        gCoordinator.SetSystemSignature<CollisionSystem>(signature);
    }
    collisionSystem->Init();

    gResourceManager.LoadShader("default",
        "Assets/Shaders/vertex.glsl",
        "Assets/Shaders/fragment.glsl");

    generateEntitiesFromMap(gCoordinator, gResourceManager, dimension, entities);

    gCoordinator.LogInfo("Entities created: ", entities.size());

    // for (auto& entity : entities)
    // {
    //     gCoordinator.LogInfo("Destroying entity ", entity);
    //     gCoordinator.DestroyEntity(entity);
    // }

    float dt = 0.0f;
    while (!sWindowManager->WindowShouldClose())
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        sWindowManager->Update(dt);

        cameraSystem->Update(dt);
        renderSystem->Update(dt);
        physicsSystem->Update(dt);

        auto stopTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::chrono::seconds::period>
             (stopTime - startTime).count();
    }
    cameraSystem->Shutdown();
    renderSystem->Shutdown();
    physicsSystem->Shutdown();

    return 0;
}

