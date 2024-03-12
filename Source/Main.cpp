#include <chrono>
#include <glm/glm.hpp>

#include "Core/Coordinator.hpp"
#include "Core/Types.hpp"

#include "Components/Transform.hpp"
#include "Components/Renderable.hpp"

#include "Systems/RenderSystem.hpp"

#define GLAD_GL_IMPLEMENTATION
#include "Core/Window/WindowManager.hpp"


Coordinator gCoordinator(LogLevel::NORMAL);

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

    auto renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Transform>());
        signature.set(gCoordinator.GetComponentType<Renderable>());
    }
    renderSystem->Init();

    Entity test = gCoordinator.CreateEntity();
    gCoordinator.AddComponent(test, Transform{});
    gCoordinator.AddComponent(test, Renderable {
        .mesh = std::make_shared<Mesh>(std::vector<Vertex> {
            Vertex(glm::vec3(0.5f, 0.0f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.5f, 0.0f)),
            Vertex(glm::vec3(0.0f, 0.0f, 0.0f))
        })
    });

    float dt = 0.0f;
    while (!sWindowManager->WindowShouldClose())
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        sWindowManager->Update(dt);
        renderSystem->Update(dt);

        auto stopTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        gCoordinator.LogDebug("fps = ", 1.0f / dt);
    }
    renderSystem->Shutdown();

    return 0;
}

