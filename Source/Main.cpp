#include "Core/Coordinator.hpp"

#define GLAD_GL_IMPLEMENTATION
#include "Core/Window/WindowManager.hpp"

const Coordinator gCoordinator(LogLevel::NORMAL);
volatile bool gRunningMainLoop = true;

void ListenerKey(Event const& event)
{
    if (event.GetParam<int>(Events::Input::Async::Key::KEY) == GLFW_KEY_ESCAPE) {
        gRunningMainLoop = false;
    }
}

int main(void)
{
    gCoordinator.AddListener(FUNCTION_LISTENER(Events::Input::Async::Key::ID, ListenerKey));

    auto windowManager = std::make_unique<WindowManager>(1920, 1080, "Game");

    while (gRunningMainLoop) {
        windowManager->Update(0);
    }

    return 0;
}

