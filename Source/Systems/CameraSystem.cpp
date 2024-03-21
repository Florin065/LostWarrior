#include <bitset>

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "Core/Event/EventTypes.hpp"
#include "Core/Coordinator.hpp"

#include "Components/Transform.hpp"
#include "Components/Camera.hpp"

#include "Systems/CameraSystem.hpp"


extern Coordinator gCoordinator;


void CameraSystem::Init()
{
    gCoordinator.AddListener(METHOD_LISTENER(
        Events::Input::Sync::Key::ID,
        CameraSystem::FreecamKeyListener));
    gCoordinator.AddListener(METHOD_LISTENER(
        Events::Input::Cursor::ID,
        CameraSystem::FreecamCursorListener));
}

void CameraSystem::Shutdown()
{

}

void CameraSystem::FreecamKeyListener(Event const& event)
{
    auto const& keys = event.GetParam<std::bitset<GLFW_KEY_LAST + 1>>
        (Events::Input::Sync::Key::KEYS);

    const float speed = 2.0f;

    for (auto const& entity : mEntities)
    {
        auto& transform = gCoordinator.GetComponent<Transform>(entity);
        auto& camera    = gCoordinator.GetComponent<Camera>(entity);

        if (keys[GLFW_KEY_W])
        {
            transform.Translate(speed * mDt * camera.direction);
        }
        if (keys[GLFW_KEY_S])
        {
            transform.Translate(speed * mDt * -camera.direction);
        }
        if (keys[GLFW_KEY_D])
        {
            transform.Translate(speed * mDt *
                                glm::cross(camera.direction, camera.up));
        }
        if (keys[GLFW_KEY_A])
        {
            transform.Translate(speed * mDt *
                                -glm::cross(camera.direction, camera.up));
        }
        if (keys[GLFW_KEY_SPACE])
        {
            transform.Translate(speed * mDt * camera.up);
        }
        if (keys[GLFW_KEY_LEFT_CONTROL])
        {
            transform.Translate(speed * mDt * -camera.up);
        }
    }
}

void CameraSystem::FreecamCursorListener(Event const& event)
{
    float dx = event.GetParam<float>(Events::Input::Cursor::DX);
    float dy = event.GetParam<float>(Events::Input::Cursor::DY);

    const float sensitivity = 0.5f;

    for (auto const& entity : mEntities)
    {
        auto& camera = gCoordinator.GetComponent<Camera>(entity);
        camera.direction = glm::rotate(camera.direction, -sensitivity * dx,
                                       camera.up);
        camera.direction = glm::rotate(camera.direction, -sensitivity * dy,
                                       glm::cross(camera.up, camera.direction));
    }
}

void CameraSystem::Update(float dt)
{
    mDt = dt;

    for (auto const& entity : mEntities)
    {
        auto& transform = gCoordinator.GetComponent<Transform>(entity);
        auto& camera    = gCoordinator.GetComponent<Camera>(entity);

        camera.view = glm::lookAt(transform.__pos,
                                  transform.__pos + camera.direction,
                                  camera.up);
        camera.projection = glm::perspective(glm::radians(60.0f),
                                             1920.0f / 1080.0f,
                                             0.1f,
                                             100.0f);

        transform.Rotate(glm::vec3(0.0f, dt * glm::radians(1.0f), 0.0f));
    }
}

