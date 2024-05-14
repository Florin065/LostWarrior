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

#define GLAD_GL_IMPLEMENTATION
#include "Core/Window/WindowManager.hpp"

Coordinator gCoordinator(LogLevel::NORMAL);
ResourceManager gResourceManager;

static auto sWindowManager = std::make_unique<WindowManager>(1920, 1080, "Game");


std::vector<std::vector<int>> generateRandomMap(int minWidth, int minHeight, int maxWidth, int maxHeight, int maxObstaclesPercentage) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> widthDis(minWidth, maxWidth);
    std::uniform_int_distribution<> heightDis(minHeight, maxHeight);

    int width = widthDis(gen);
    int height = heightDis(gen);

    std::vector<std::vector<int>> map(height, std::vector<int>(width, 0));

    // Add walls
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                if ((i == 0 && j == 0) || (i == 0 && j == width - 1) || (i == height - 1 && j == 0) || (i == height - 1 && j == width - 1)) {
                    map[i][j] = 2;  // Corner
                } else if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                    map[i][j] = 4;  // Wall up, down, left, right
                }
            }
        }
    }

    // Add exits (6 - Up, 7 - Right, 8 - Down, 9 - Left)
    std::uniform_int_distribution<> exitDis(1, 3);
    int exitCount = exitDis(gen);
    std::vector<int> exitTypes = {6, 7, 8, 9};
    std::shuffle(exitTypes.begin(), exitTypes.end(), gen);

    for (int i = 0; i < exitCount; ++i) {
        int side = exitTypes[i]; // Up, Right, Down, Left
        switch (side) {
            case 6: // Up
                map[0][rand() % (width - 2) + 1] = 6;
                break;
            case 7: // Right
                map[rand() % (height - 2) + 1][width - 1] = 7;
                break;
            case 8: // Down
                map[height - 1][rand() % (width - 2) + 1] = 8;
                break;
            case 9: // Left
                map[rand() % (height - 2) + 1][0] = 9;
                break;
        }
    }

    int maxObstacles = (width - 2) * (height - 2) * maxObstaclesPercentage / 100;

    std::uniform_int_distribution<> obstacleDis(10, 13);
    int obstacleCount = obstacleDis(gen);
    obstacleCount = std::min(obstacleCount, maxObstacles);

    while (obstacleCount > 0) {
        int row = rand() % (height - 2) + 1;
        int col = rand() % (width - 2) + 1;
        if (map[row][col] == 0) {
            map[row][col] = obstacleDis(gen);
            --obstacleCount;
        }
    }

    return map;
}

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
       std::make_shared<Model>("Assets/Dungeon/wall.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable wall = Renderable {
       std::make_shared<Model>("Assets/Dungeon/wall-narrow.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable tile = Renderable {
       std::make_shared<Model>("Assets/Dungeon/floor.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable door = Renderable {
       std::make_shared<Model>("Assets/Dungeon/gate.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable opening = Renderable {
       std::make_shared<Model>("Assets/Dungeon/wall-opening.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable trap = Renderable {
       std::make_shared<Model>("Assets/Dungeon/trap.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable barrel = Renderable {
       std::make_shared<Model>("Assets/Dungeon/barrel.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable rocks = Renderable {
       std::make_shared<Model>("Assets/Dungeon/rocks.obj"),
       gResourceManager.GetShader("default") 
    };

    Renderable stones = Renderable {
       std::make_shared<Model>("Assets/Dungeon/stones.obj"),
       gResourceManager.GetShader("default") 
    };

    std::vector<std::vector<int>> map = generateRandomMap(8, 9, 15, 15, 20);

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
            if (map[i][j] == 6 || map[i][j] == 7 || map[i][j] == 8 || map[i][j] == 9)
            {
                transform.Scale({0.4f, 0.4f, 0.4f});
            }
            if (map[i][j] == 7 || map[i][j] == 8)
            {
                transform.Rotate({0.0f, glm::radians(180.0f), 0.0f});
            }
            if (map[i][j] == 9)
            {
                transform.Rotate({0.0f, glm::radians(360.0f), 0.0f});
            }

            switch (map[i][j]) {
            case 2:
                gCoordinator.AddComponent(entity, corner);
                break;
            case 4:
            case 5:
                gCoordinator.AddComponent(entity, wall);
                break;
            case 6:
            case 7:
            case 8:
            case 9:
                gCoordinator.AddComponent(entity, door);
                break;
            case 10:
                gCoordinator.AddComponent(entity, trap);
                break;
            case 11:
                gCoordinator.AddComponent(entity, barrel);
                break;
            case 12:
                gCoordinator.AddComponent(entity, rocks);
                break;
            case 13:
                gCoordinator.AddComponent(entity, stones);
                break;
            }
            Entity floor = gCoordinator.CreateEntity();
            Transform floor_transform;
            floor_transform.Translate({i, 0.0f, j});
            gCoordinator.AddComponent(floor, tile);
            gCoordinator.AddComponent(floor, floor_transform);

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

