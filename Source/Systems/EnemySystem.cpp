#include "Components/Camera.hpp"
#include "Components/Enemy.hpp"
#include <bitset>

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "Core/Event/EventTypes.hpp"
#include "Core/Coordinator.hpp"

#include "Components/Transform.hpp"
#include "Components/RigidBody.hpp"

#include "Systems/EnemySystem.hpp"


extern Coordinator gCoordinator;


void EnemySystem::Init()
{

}

void EnemySystem::Shutdown()
{

}

void EnemySystem::Update(float dt)
{
    for (auto const& entity : mEntities)
    {
        auto& rigidBody = gCoordinator.GetComponent<RigidBody>(entity);
        auto& transform = gCoordinator.GetComponent<Transform>(entity);
        auto& enemy     = gCoordinator.GetComponent<Enemy>(entity);

        glm::vec3 playerPos = gCoordinator.GetComponent<Transform>(enemy.player).__pos;
        glm::vec3 enemyPos = transform.__pos;

        glm::vec3 dir = glm::normalize(playerPos - enemyPos);

        gCoordinator.LogInfo("dir: ", dir.x, " ", dir.y, " ", dir.z);
        rigidBody.velocity = dir * 0.3f;
    }
}