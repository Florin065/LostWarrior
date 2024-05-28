#include <chrono>
#include <glm/glm.hpp>
#include <random>

#include "Components/Player.hpp"
#include "Components/Enemy.hpp"
#include "Core/Coordinator.hpp"
#include "Core/Types.hpp"
#include "Core/ResourceManager.hpp"

#include "Components/Transform.hpp"
#include "Components/Renderable.hpp"
#include "Components/Camera.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Collider.hpp"

#include "Systems/CollisionSystem.hpp"
#include "Systems/PlayerSystem.hpp"
#include "Systems/EnemySystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/MapGenerationSystem.hpp"

#define GLAD_GL_IMPLEMENTATION
#include "Core/Window/WindowManager.hpp"

Coordinator gCoordinator(LogLevel::NORMAL);
ResourceManager gResourceManager;

int dimension = 1;
Entity player;
std::vector<Entity> entities;
std::vector<Entity> doors;
int completion = 0;

static auto sWindowManager = std::make_unique<WindowManager>(1920, 1080, "Game");


void listenerKey(Event const& event)
{
    if (event.GetParam<int>(Events::Input::Async::Key::KEY) == GLFW_KEY_ESCAPE)
    {
        sWindowManager->SetWindowShouldClose();
    }
}

void listenerRoomCleared([[maybe_unused]] Event const& event)
{
    if (completion <= 20)
    {
        for (auto& entity: doors)
        {
            gCoordinator.LogInfo("Changing door with id ", entity);
            Renderable& doorRenderable = gCoordinator.GetComponent<Renderable>(entity);
            doorRenderable.model = gResourceManager.GetModel("Assets/Dungeon/wall-opening.obj");
            Collider& doorCol = gCoordinator.GetComponent<Collider>(entity);
            doorCol.width = 1.0f;
            doorCol.length = 1.0f;
            doorCol.name = "open_door";
        }
    }
    else
    {
        printf("You win!\n");
        exit(69);
    }
    
    gCoordinator.LogInfo("Entities created: ", entities.size());
}

void enemyKilledUpdateEntities(Event const& event)
{
    gCoordinator.LogInfo("Enemy killed!");
    Entity enemy = event.GetParam<Entity>(Events::Game::KilledEnemy::ENTITY_ID);
    entities.erase(std::remove(entities.begin(), entities.end(), enemy), entities.end());
}


int main(void)
{

    gCoordinator.AddListener(FUNCTION_LISTENER(Events::Input::Async::Key::ID,
                                               listenerKey));
    gCoordinator.AddListener(FUNCTION_LISTENER(Events::Game::RoomCleared::ID,
                                               listenerRoomCleared));
    gCoordinator.AddListener(FUNCTION_LISTENER(Events::Game::KilledEnemy::ID,
                                               enemyKilledUpdateEntities));

    gCoordinator.RegisterComponent<Transform>();
    gCoordinator.RegisterComponent<Renderable>();
    gCoordinator.RegisterComponent<Camera>();
    gCoordinator.RegisterComponent<RigidBody>();
    gCoordinator.RegisterComponent<Collider>();
    gCoordinator.RegisterComponent<Player>();
    gCoordinator.RegisterComponent<Enemy>();

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

    auto playerSystem = gCoordinator.RegisterSystem<PlayerSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<RigidBody>());
        signature.set(gCoordinator.GetComponentType<Renderable>());
        signature.set(gCoordinator.GetComponentType<Player>());
        gCoordinator.SetSystemSignature<PlayerSystem>(signature);
    }
    playerSystem->Init();

    auto enemySystem = gCoordinator.RegisterSystem<EnemySystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<RigidBody>());
        signature.set(gCoordinator.GetComponentType<Renderable>());
        signature.set(gCoordinator.GetComponentType<Enemy>());
        gCoordinator.SetSystemSignature<EnemySystem>(signature);
    }
    enemySystem->Init();

    gResourceManager.LoadShader("default",
        "Assets/Shaders/vertex.glsl",
        "Assets/Shaders/fragment.glsl");

    Entity camera = gCoordinator.CreateEntity();
    {
        Transform cameraT;
        cameraT.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        Camera cameraC;
        gCoordinator.AddComponent(camera, cameraT);
        gCoordinator.AddComponent(camera, cameraC);
    }

    player = gCoordinator.CreateEntity();
    {
        Transform playerT = Transform {};
        playerT.SetScale(glm::vec3(1.0f));
        gCoordinator.AddComponent(player, playerT);
        gCoordinator.AddComponent(player, RigidBody {
            .drag = 0.04f
        });
        gCoordinator.AddComponent(player, Renderable {
            gResourceManager.GetModel("Assets/kenney_graveyard-kit/Models/OBJ format/character-vampire.obj"),
            gResourceManager.GetShader("default") 
        });
        gCoordinator.AddComponent(player, Player { camera });
        gCoordinator.AddComponent(player, Collider { .name = "player", .layer = ColliderLayer::COLLIDER_PHYSICAL, .length = 0.5f, .width = 0.5f, .health = 10.0f, .damage = 0.0f});
        renderSystem->SwapCamera(camera);
    }

    generateEntitiesFromMap(gCoordinator, gResourceManager, dimension, entities, player, completion, doors);

    gCoordinator.LogInfo("Entities created: ", entities.size());

    float dt = 0.0f;
    while (!sWindowManager->WindowShouldClose())
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        sWindowManager->Update(dt);

        cameraSystem->Update(dt);
        renderSystem->Update(dt);
        physicsSystem->Update(dt);
        playerSystem->Update(dt);
        collisionSystem->Update(dt);
        enemySystem->Update(dt);

        auto stopTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::chrono::seconds::period>
             (stopTime - startTime).count();
    }
    cameraSystem->Shutdown();
    renderSystem->Shutdown();
    physicsSystem->Shutdown();

    return 0;
}

