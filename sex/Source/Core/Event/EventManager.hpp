#pragma once

#include <unordered_map>
#include <functional>
#include <list>

#include "Core/Event/EventTypes.hpp"
#include "Core/Event/Event.hpp"


class EventManager
{
public:
    void AddListener(EventId id, std::function<void(Event const&)> const& listener)
    {
        mListeners[id].push_back(listener);
    }

    void SendEvent(Event const& event)
    {
        const EventId id = event.GetId();

        for (auto const& listener : mListeners[id]) {
            listener(event);
        }
    }

    void SendEvent(const EventId id)
    {
        Event event(id);

        for (auto const& listener : mListeners[id]) {
            listener(event);
        }
    }

private:
    std::unordered_map<EventId, std::list<std::function<void(Event const&)>>> mListeners;
};

