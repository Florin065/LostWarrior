#include <chrono>
#include <glm/glm.hpp>

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

#define GLAD_GL_IMPLEMENTATION
#include "Core/Window/WindowManager.hpp"

Coordinator gCoordinator(LogLevel::NORMAL);
ResourceManager gResourceManager;

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

#if 1
    gResourceManager.LoadShader("default",
        "Assets/Shaders/vertex.glsl",
        "Assets/Shaders/fragment.glsl");

    Renderable corner = Renderable {
       std::make_shared<Model>("Assets/Kenney/Models/OBJformat/wall.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable wall = Renderable {
       std::make_shared<Model>("Assets/Kenney/Models/OBJformat/wall-narrow.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable tile = Renderable {
       std::make_shared<Model>("Assets/Kenney/Models/OBJformat/floor.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable door = Renderable {
       std::make_shared<Model>("Assets/Kenney/Models/OBJformat/wall-opening.obj"),
       gResourceManager.GetShader("default") 
    };

    std::vector<std::vector<int>> map {
        {2, 4, 4, 4, 4, 6, 4, 4, 2},
        {5, 0, 0, 0, 0, 0, 0, 0, 5},
        {5, 0, 0, 0, 0, 0, 0, 0, 5},
        {5, 0, 0, 0, 0, 0, 0, 0, 5},
        {5, 0, 0, 0, 0, 0, 0, 0, 5},
        {5, 0, 0, 0, 0, 0, 0, 0, 7},
        {5, 0, 0, 0, 0, 0, 0, 0, 5},
        {2, 4, 4, 4, 4, 4, 4, 4, 2},
    };

    for (size_t i = 0; i < map.size(); ++i)
    {
        for (size_t j = 0; j < map[0].size(); j++)
        {
            Entity entity = gCoordinator.CreateEntity();

            Transform transform;
            transform.Translate({i, 0.0f, j});
            if (map[i][j] % 2 == 0)
            {
                transform.Rotate({0.0f, glm::radians(90.0f), 0.0f});
            }
            switch (map[i][j] / 2)
            {
            case 0:
                gCoordinator.AddComponent(entity, tile);
                break;
            case 1:
                gCoordinator.AddComponent(entity, corner);
                break;
            case 2:
                gCoordinator.AddComponent(entity, wall);
                break;
            case 3:
                gCoordinator.AddComponent(entity, door);
                break;
            }
            gCoordinator.AddComponent(entity, transform);
        }
    }
#endif

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

