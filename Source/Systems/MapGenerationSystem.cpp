#include <glm/glm.hpp>
#include <random>
#include <algorithm>

#include "Components/Camera.hpp"
#include "Components/Collider.hpp"
#include "Components/Player.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/Renderable.hpp"
#include "Core/Coordinator.hpp"
#include "Core/ResourceManager.hpp"
#include "Components/Enemy.hpp"


std::vector<std::vector<int>> generateRandomMap(int minWidth, int minHeight, int maxWidth, int maxHeight, int maxObstaclesPercentage, int maxEnemiesPercentage) {
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
                } else if (i == 0 || i == height - 1) {
                    map[i][j] = 4;  // Wall top and bottom
                } else if (j == 0 || j == width - 1) {
                    map[i][j] = 5;  // Wall left and right
                }
            }
        }
    }

    // Add exits (6 - Up, 7 - Right, 8 - Down, 9 - Left)
    std::uniform_int_distribution<> exitDis(2, 4);
    int exitCount = exitDis(gen);
    std::vector<int> exitTypes = {6, 7, 8, 9};
    std::shuffle(exitTypes.begin(), exitTypes.end(), gen);

    std::vector<std::pair<int, int>> exitPositions;

    for (int i = 0; i < exitCount; ++i) {
        int side = exitTypes[i]; // Up, Right, Down, Left
        switch (side) {
            case 6: { // Up
                int pos = rand() % (width - 2) + 1;
                map[0][pos] = 6;
                exitPositions.push_back({0, pos});
                break;
            }
            case 7: { // Right
                int pos = rand() % (height - 2) + 1;
                map[pos][width - 1] = 7;
                exitPositions.push_back({pos, width - 1});
                break;
            }
            case 8: { // Down
                int pos = rand() % (width - 2) + 1;
                map[height - 1][pos] = 8;
                exitPositions.push_back({height - 1, pos});
                break;
            }
            case 9: { // Left
                int pos = rand() % (height - 2) + 1;
                map[pos][0] = 9;
                exitPositions.push_back({pos, 0});
                break;
            }
        }
    }

    int maxObstacles = (width - 2) * (height - 2) * maxObstaclesPercentage / 100;

    std::uniform_int_distribution<> obstacleDis(10, 13);
    int obstacleCount = obstacleDis(gen);
    obstacleCount = std::min(obstacleCount, maxObstacles);

    while (obstacleCount > 0) {
        int row = rand() % (height - 2) + 1;
        int col = rand() % (width - 2) + 1;

        bool adjacentToExit = false;
        for (const auto& exitPos : exitPositions) {
            if (std::abs(exitPos.first - row) <= 1 && std::abs(exitPos.second - col) <= 1) {
                adjacentToExit = true;
                break;
            }
        }

        if (!adjacentToExit && map[row][col] == 0) {
            map[row][col] = obstacleDis(gen);
            --obstacleCount;
        }
    }

    // Add enemies (20 - 23)
    int maxEnemies = (width - 2) * (height - 2) * maxEnemiesPercentage / 100;
    std::uniform_int_distribution<> enemyDis(15, 16);
    int enemyCount = std::uniform_int_distribution<>(1, maxEnemies)(gen);

    while (enemyCount > 0) {
        int row = rand() % (height - 2) + 1;
        int col = rand() % (width - 2) + 1;

        if (map[row][col] == 0) {
            map[row][col] = enemyDis(gen);
            --enemyCount;
        }
    }

    return map;
}

void generateEntitiesFromMap(Coordinator& gCoordinator,
                             ResourceManager& gResourceManager,
                             int dimension,
                             std::vector<Entity>& entities,
                             Entity player,
                             int completion,
                             std::vector<Entity>& doors)
{
    Renderable corner, wall, tile, door, opening, trap, object, rocks, stones, enemy_digger, enemy_skeleton;

    std::vector<std::vector<int>> map = generateRandomMap(8, 9, 15, 15, 60, completion + 5);


    bool playerSpawned = false;

    if (dimension == 1) {
        corner = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/wall.obj"),
        gResourceManager.GetShader("default") 
        };

        wall = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/wall-narrow.obj"),
        gResourceManager.GetShader("default") 
        };

        tile = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/floor.obj"),
        gResourceManager.GetShader("default") 
        };

        door = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/gate.obj"),
        gResourceManager.GetShader("default") 
        };

        opening = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/wall-opening.obj"),
        gResourceManager.GetShader("default") 
        };

        trap = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/trap.obj"),
        gResourceManager.GetShader("default") 
        };

        object = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/barrel.obj"),
        gResourceManager.GetShader("default") 
        };

        rocks = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/rocks.obj"),
        gResourceManager.GetShader("default") 
        };

        stones = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/stones.obj"),
        gResourceManager.GetShader("default") 
        };

        enemy_digger = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/character-ghost.obj"),
        gResourceManager.GetShader("default") 
        };

        enemy_skeleton = Renderable {
        gResourceManager.GetModel("Assets/Dungeon/character-zombie.obj"),
        gResourceManager.GetShader("default") 
        };

    } else {
        int rows = map.size();
        int cols = map[0].size();

        map[0][0] = 3;
        map[0][cols - 1] = 14;
        map[rows - 1][0] = 2;
        map[rows - 1][cols - 1] = 1;

        corner = Renderable {
        gResourceManager.GetModel("Assets/Arena/wall-corner.obj"),
        gResourceManager.GetShader("default") 
        };

        wall = Renderable {
        gResourceManager.GetModel("Assets/Arena/wall.obj"),
        gResourceManager.GetShader("default") 
        };

        tile = Renderable {
        gResourceManager.GetModel("Assets/Arena/floor.obj"),
        gResourceManager.GetShader("default") 
        };

        door = Renderable {
        gResourceManager.GetModel("Assets/Arena/wall-gate.obj"),
        gResourceManager.GetShader("default") 
        };

        opening = Renderable {
        gResourceManager.GetModel("Assets/Arena/wall-gate.obj"),
        gResourceManager.GetShader("default") 
        };

        trap = Renderable {
        gResourceManager.GetModel("Assets/Arena/weapon-spear.obj"),
        gResourceManager.GetShader("default") 
        };

        object = Renderable {
        gResourceManager.GetModel("Assets/Arena/statue.obj"),
        gResourceManager.GetShader("default") 
        };

        rocks = Renderable {
        gResourceManager.GetModel("Assets/Arena/column-damaged.obj"),
        gResourceManager.GetShader("default") 
        };

        stones = Renderable {
        gResourceManager.GetModel("Assets/Arena/bricks.obj"),
        gResourceManager.GetShader("default") 
        };

        enemy_digger = Renderable {
        gResourceManager.GetModel("Assets/Arena/character-digger.obj"),
        gResourceManager.GetShader("default") 
        };

        enemy_skeleton = Renderable {
        gResourceManager.GetModel("Assets/Arena/character-skeleton.obj"),
        gResourceManager.GetShader("default") 
        };
    }

    for (size_t i = 0; i < map.size(); ++i)
    {
        for (size_t j = 0; j < map[0].size(); j++)
        {
            Entity floor = gCoordinator.CreateEntity();
            Transform floor_transform;
            floor_transform.Translate({i, 0.0f, j});
            gCoordinator.AddComponent(floor, tile);
            gCoordinator.AddComponent(floor, floor_transform);
            entities.push_back(floor);

            if (map[i][j] == 0)
            {
                continue;
            }

            Entity entity = gCoordinator.CreateEntity();

            Transform transform;
            transform.Translate({i, 0.0f, j});
            RigidBody rb = RigidBody { .mass = 99999.0f };
            Collider col = Collider { .name = "entity" + std::to_string(entity), .layer = ColliderLayer::COLLIDER_PHYSICAL, .length = 1.0f, .width = 1.0f};

            if (map[i][j] % 2 == 0)
            {
                transform.Rotate({0.0f, glm::radians(90.0f), 0.0f});
            }
            if (map[i][j] == 6 || map[i][j] == 7 || map[i][j] == 8 || map[i][j] == 9)
            {
                transform.Scale({0.4f, 0.4f, 0.4f});
            }
            if (map[i][j] == 7 || map[i][j] == 8 || map[i][j] == 3 || map[i][j] == 14)
            {
                transform.Rotate({0.0f, glm::radians(180.0f), 0.0f});
            }
            if (map[i][j] == 9)
            {
                transform.Rotate({0.0f, glm::radians(360.0f), 0.0f});
            }

            switch (map[i][j]) {
            case 1:
            case 2:
            case 3:
            case 14:
                gCoordinator.AddComponent(entity, corner);
                col.name = "corner" + std::to_string(entity);
                col.length = 1.5f; col.width = 1.5f, col.damage = 0.0f; col.health = 99999.0f;
                break;
            case 4:
            case 5:
                gCoordinator.AddComponent(entity, wall);
                col.name = "wall" + std::to_string(entity);
                col.length = 1.5f; col.width = 1.5f , col.damage = 0.0f; col.health = 99999.0f;
                break;
            case 6:
            case 7:
            case 8:
            case 9:
                if (!playerSpawned)
                {
                    auto& playerT = gCoordinator.GetComponent<Transform>(player);
                    auto& playerRb = gCoordinator.GetComponent<RigidBody>(player);
                    auto& playerP = gCoordinator.GetComponent<Player>(player);

                    glm::vec3 playerPos;

                    if (i == 0)
                    {
                        playerPos = glm::vec3(i + 1, 0.0f, j);
                    }
                    else if (i == map.size() - 1)
                    {
                        playerPos = glm::vec3(i - 1, 0.0f, j);
                    }
                    else if (j == 0)
                    {
                        playerPos = glm::vec3(i, 0.0f, j + 1);
                    }
                    else if (j == map[0].size() - 1)
                    {
                        playerPos = glm::vec3(i, 0.0f, j - 1);
                    }

                    playerT.SetPosition({playerPos});
                    playerRb.velocity = glm::vec3(0.0f);

                    auto& cameraC = gCoordinator.GetComponent<Camera>(playerP.camera);

                    glm::vec3 mapCenter = glm::vec3(map.size() / 2, 0, map[0].size() / 2);
                    glm::vec3 playerCamPos = glm::vec3(i, 0, j);
                    glm::vec3 dirXZ = glm::normalize(mapCenter - playerCamPos);
                    dirXZ.y -= 1.0f;
                    cameraC.direction = glm::normalize(dirXZ);

                    playerSpawned = true;
                    gCoordinator.AddComponent(entity, door);
                } else {
                    gCoordinator.AddComponent(entity, door);
                    doors.push_back(entity);
                    gCoordinator.LogInfo("Added door ", entity);
                }
                col.name = "door" + std::to_string(entity);
                col.damage = 0.0f; col.health = 99999.0f;
                break;
            case 10:
                gCoordinator.AddComponent(entity, trap);
                col.name = "trap" + std::to_string(entity);
                col.health = 99999.0f;
                break;
            case 11:
                gCoordinator.AddComponent(entity, object);
                col.name = "object" + std::to_string(entity);
                col.damage = 0.0f; col.health = 99999.0f;
                break;
            case 12:
                gCoordinator.AddComponent(entity, rocks);
                col.name = "rocks" + std::to_string(entity);
                col.damage = 0.0f; col.health = 99999.0f;
                break;
            case 13:
                gCoordinator.AddComponent(entity, stones);
                col.name = "stones" + std::to_string(entity);
                col.damage = 0.0f; col.health = 99999.0f;
                break;
            case 15:
                gCoordinator.AddComponent(entity, enemy_digger);
                gCoordinator.AddComponent(entity, Enemy { player });
                col.name = "digger" + std::to_string(entity);
                col.length = 0.5f;
                col.width = 0.5f;
                rb.angularVel = glm::vec3(0.0f, 1.0f, 0.0f);
                break;
            case 16:
                gCoordinator.AddComponent(entity, enemy_skeleton);
                gCoordinator.AddComponent(entity, Enemy { player });
                col.name = "skeleton" + std::to_string(entity);
                col.length = 0.5f;
                col.width = 0.5f;
                rb.angularVel = glm::vec3(0.0f, 1.5f, 0.0f);
                break;
            default:
                col.name = "unknown" + std::to_string(map[i][j]);
            }
            gCoordinator.AddComponent(entity, rb);
            gCoordinator.AddComponent(entity, col);
            gCoordinator.AddComponent(entity, transform);
            entities.push_back(entity);
        }
    }
    gCoordinator.LogInfo("Created map");
}

