#include "Components/Camera.hpp"
#include "Components/Player.hpp"
#include <bitset>
#include <Core/ResourceManager.hpp>

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "Core/Event/EventTypes.hpp"
#include "Core/Coordinator.hpp"

#include "Components/Transform.hpp"
#include "Components/RigidBody.hpp"

#include "Systems/PlayerSystem.hpp"
#include <Components/Renderable.hpp>
#include <Components/Collider.hpp>


extern Coordinator gCoordinator;
extern ResourceManager gResourceManager;


void PlayerSystem::Init()
{
    gCoordinator.AddListener(METHOD_LISTENER(
        Events::Input::Sync::Key::ID,
        PlayerSystem::KeyListener));
    gCoordinator.AddListener(METHOD_LISTENER(
        Events::Input::Cursor::ID,
        PlayerSystem::CursorListener));
}

void PlayerSystem::Shutdown()
{

}

void PlayerSystem::KeyListener(Event const& event)
{
    auto const& keys = event.GetParam<std::bitset<GLFW_KEY_LAST + 1>>
        (Events::Input::Sync::Key::KEYS);

    for (auto const& entity : mEntities)
    {
        auto& transform = gCoordinator.GetComponent<Transform>(entity);
        auto& rigidBody = gCoordinator.GetComponent<RigidBody>(entity);
        auto& playerP = gCoordinator.GetComponent<Player>(entity);

        glm::vec3 dir = glm::vec3(0.0f);

        if (keys[GLFW_KEY_W])
        {
            dir += playerP.direction;
        }
        if (keys[GLFW_KEY_S])
        {
            dir += -playerP.direction;
        }
        if (keys[GLFW_KEY_D])
        {
            dir += glm::cross(playerP.direction, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (keys[GLFW_KEY_A])
        {
            dir += -glm::cross(playerP.direction, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        if (dir != glm::vec3(0.0f))
        {
            rigidBody.forces["Push"] = 20.0f * glm::normalize(dir);
        }

        if (keys[GLFW_KEY_SPACE] && playerP.cooldown <= 0.0f)
        {
            Entity arrow = gCoordinator.CreateEntity();
            Transform arrowTransform = transform;
            RigidBody arrowRigidBody = rigidBody;
            arrowRigidBody.velocity += 3.0f * playerP.direction;
            arrowRigidBody.angularVel = glm::vec3(0.0f, 10.0f, 0.0f);
            arrowRigidBody.drag = 0.0001f;
            arrowTransform.Translate(playerP.direction);
            arrowTransform.SetScale(glm::vec3(0.5f));
            gCoordinator.AddComponent<Transform>(arrow, arrowTransform);
            gCoordinator.AddComponent<RigidBody>(arrow, arrowRigidBody);
            Renderable arrowRenderable = Renderable {
                gResourceManager.GetModel("Assets/kenney_graveyard-kit/Models/OBJ format/cross.obj"),
                gResourceManager.GetShader("default") 
            };
            
            gCoordinator.AddComponent<Renderable>(arrow, arrowRenderable);
            gCoordinator.AddComponent<Collider>(arrow, Collider {
                .name = "arrow",
                .layer = ColliderLayer::COLLIDER_PHYSICAL,
                .length = 0.1f,
                .width = 0.1f,
                .health = 1.0f,
                .damage = 0.5f,
                .projectile = true
            });
            playerP.cooldown = PLAYER_ARROW_CD;
        }
    }
}

void PlayerSystem::CursorListener(Event const& event)
{
    float dx = event.GetParam<float>(Events::Input::Cursor::DX);
    float dy = event.GetParam<float>(Events::Input::Cursor::DY);

    for (auto const& entity : mEntities)
    {
        (void) entity;
    }

    (void) dx;
    (void) dy;
}

void PlayerSystem::Update(float dt)
{
    mDt = dt;

    for (auto const& entity : mEntities)
    {
        auto& rigidBody = gCoordinator.GetComponent<RigidBody>(entity);
        auto& transform = gCoordinator.GetComponent<Transform>(entity);
        auto& player    = gCoordinator.GetComponent<Player>(entity);
        rigidBody.forces["Push"] = glm::vec3(0.0f);

        if (player.camera != MAX_ENTITIES + 1)
        {
            auto& cameraT = gCoordinator.GetComponent<Transform>(player.camera);
            auto& cameraC = gCoordinator.GetComponent<Camera>(player.camera);
            cameraT.SetPosition(transform.__pos - 4.0f * cameraC.direction + glm::vec3(0.0f, 1.0f, 0.0f));

            player.direction = cameraC.direction;
            player.direction.y = 0.0f;
            player.direction = glm::normalize(player.direction);

            float angle = glm::acos(glm::dot(player.direction, glm::vec3(0.0f, 0.0f, 1.0f)));
            if ((glm::vec3(0.0f, 0.0f, 1.0f) - player.direction).x >= 0)
            {
                angle = 2 * glm::pi<float>() - angle;
            }
            transform.SetRotation({0.0f, angle ,0.0f});
        }

        player.cooldown -= dt;
    }
}

