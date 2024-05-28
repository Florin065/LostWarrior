#pragma once

#include <bitset>
#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glad/gl.h"

#include "Core/Coordinator.hpp"


extern Coordinator gCoordinator;


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
        glfwSetCursorPosCallback(mWindow.get(), CallbackCursorPos);
        glfwSetFramebufferSizeCallback(mWindow.get(), CallbackFramebufferSize);

        glfwSwapInterval(1);
        
        glfwSetInputMode(mWindow.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glViewport(0, 0, width, height);
        gCoordinator.LogInfo("Viewport set to (", width, " ", height, ")");

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(CallbackMessage, 0);
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

    static void CallbackCursorPos(GLFWwindow* window, double xpos, double ypos)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        auto map = [](float x, float l, float a) { return (x / l) * 2 * a - a; };

        const float mappedX = map(xpos, width, static_cast<float>(width) / height);
        const float mappedY = map(ypos, height, -1);

        static float lastMappedX = mappedX;
        static float lastMappedY = mappedY;

        Event event(Events::Input::Cursor::ID);
        event.SetParam(Events::Input::Cursor::XPOS, mappedX);
        event.SetParam(Events::Input::Cursor::YPOS, mappedY);
        event.SetParam(Events::Input::Cursor::DX, mappedX - lastMappedX);
        event.SetParam(Events::Input::Cursor::DY, mappedY - lastMappedY);
        gCoordinator.SendEvent(event);

        lastMappedX = mappedX;
        lastMappedY = mappedY;
    }

    static void GLAPIENTRY CallbackMessage([[maybe_unused]] GLenum source,
                                           GLenum type,
                                           [[maybe_unused]] GLuint id,
                                           GLuint severity,
                                           [[maybe_unused]] GLsizei length,
                                           const char* message,
                                           [[maybe_unused]] const void* userParam)
    {
        if (type == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_SEVERITY_HIGH)
        {
            gCoordinator.LogError("GL Error: ", message);
        }
        else
        {
            switch (severity) {
            case GL_DEBUG_SEVERITY_MEDIUM:
                gCoordinator.LogInfo("GL Warning(major): ", message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                gCoordinator.LogInfo("GL Warning(minor): ", message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                gCoordinator.LogDebug("GL Notification: ", message);
                break;
            default:
                #if defined(_MSC_VER) && !defined(__clang__) // MSVC
                    __assume(false);
                #else
                    __builtin_unreachable();
                #endif
            }
        }
    }
};

