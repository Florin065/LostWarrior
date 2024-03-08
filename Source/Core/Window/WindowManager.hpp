#pragma once

#include <bitset>
#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glad/gl.h"

#include "Core/Coordinator.hpp"


extern const Coordinator gCoordinator;


class WindowManager
{
public:
    WindowManager(const int width, const int height, std::string const& title)
    {
        glfwSetErrorCallback(CallbackError);
        gCoordinator.Assert(glfwInit(), "glfw failed to initialize");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        mWindow.reset(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));
        gCoordinator.Assert(mWindow.get(), "Window creation failed");

        glfwMakeContextCurrent(mWindow.get());
        gladLoadGL(glfwGetProcAddress); 

        glfwSetKeyCallback(mWindow.get(), CallbackKey);
        glfwSetFramebufferSizeCallback(mWindow.get(), CallbackFramebufferSize);

        glViewport(0, 0, width, height);
        gCoordinator.LogInfo("Viewport set to (", width, " ", height, ")");
    }

    ~WindowManager()
    {
        mWindow.reset();
        glfwTerminate();
    }

    void Update([[maybe_unused]] float dt)
    {
        ProcessInput();

        glfwSwapBuffers(mWindow.get());
        glfwPollEvents();
    }

    int WindowShouldClose() const
    {
        return glfwWindowShouldClose(mWindow.get());
    }

    void SetWindowShouldClose()
    {
        glfwSetWindowShouldClose(mWindow.get(), GLFW_TRUE);
    }

private:
    struct GLFWwindowDestroy
    {
        void operator()(GLFWwindow* ptr)
        {
            glfwDestroyWindow(ptr);
        }
    };
    std::unique_ptr<GLFWwindow, GLFWwindowDestroy> mWindow;

    void ProcessInput()
    {
        std::bitset<GLFW_KEY_LAST + 1> keys;

        for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
            keys[key] = glfwGetKey(mWindow.get(), key);
        }

        gCoordinator.SendEvent(Event(Events::Input::Sync::Key::ID)
                    .SetParam(Events::Input::Sync::Key::KEYS, keys));
    }

    static void CallbackError([[maybe_unused]] int error, const char* description)
    {
        gCoordinator.LogError(description);
    }

    static void CallbackFramebufferSize([[maybe_unused]] GLFWwindow* window,
                                        int width, int height)
    {
        glViewport(0, 0, width, height);
        gCoordinator.LogInfo("Viewport set to (", width, " ", height, ")");
        gCoordinator.SendEvent(Event(Events::Window::Resize::ID)
                    .SetParam(Events::Window::Resize::WIDTH, width)
                    .SetParam(Events::Window::Resize::HEIGHT, height));
    }

    static void CallbackKey([[maybe_unused]] GLFWwindow* window,
                            int key, int scancode, int action, int mods)
    {
        gCoordinator.LogDebug("Key pressed: ", static_cast<char>(key));
        gCoordinator.SendEvent(Event(Events::Input::Async::Key::ID)
                    .SetParam(Events::Input::Async::Key::KEY, key)
                    .SetParam(Events::Input::Async::Key::SCANCODE, scancode)
                    .SetParam(Events::Input::Async::Key::ACTION, action)
                    .SetParam(Events::Input::Async::Key::MODS, mods));
    }
};

