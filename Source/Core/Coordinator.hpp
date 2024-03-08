#pragma once

#include <memory>

#include "Core/Event/EventManager.hpp"
#include "Core/Io/LogManager.hpp"

class Coordinator
{
public:
    Coordinator(LogLevel logLevel)
        : mLogManager(std::make_unique<LogManager>(logLevel)),
          mEventManager(std::make_unique<EventManager>())
    { }

    // EventManager Methods
    void AddListener(EventId id, std::function<void(Event const&)> const& listener) const
    {
        mEventManager.get()->AddListener(id, listener);
    }

    void SendEvent(Event const& event) const
    {
        mEventManager.get()->SendEvent(event);
    }

    void SendEvent(const EventId id) const
    {
        mEventManager.get()->SendEvent(id);
    }

    // LogManager Methods
    template <typename... Args>
    void LogDebug(Args&&... args) const
    {
        mLogManager->Debug(args...);
    }

    template <typename... Args>
    void LogInfo(Args&&... args) const
    {
        mLogManager->Info(args...);
    }

    template <typename... Args>
    void LogError(Args&&... args) const
    {
        mLogManager->Error(args...);
    }

    template<typename T, typename... Args>
    void Assert(T condition, Args&&... args) const
    {
        mLogManager->Assert(condition, args...);
    }

private:
    const std::unique_ptr<LogManager> mLogManager;
    const std::unique_ptr<EventManager> mEventManager;
};

