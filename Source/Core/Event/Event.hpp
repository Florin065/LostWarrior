#pragma once

#include <any>
#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <unordered_map>

#include "Core/Event/EventTypes.hpp"


class Event
{
public:
    Event() = delete;

    explicit Event(EventId id)
        : mId(id)
    {}

    template<typename T>
    Event& SetParam(ParamId id, T value)
    {
        mData[id] = EventData(value, typeid(value).hash_code());
        return *this;
    }

    template<typename T>
    T GetParam(ParamId id) const
    {
        EventData param = mData.at(id);
        if (typeid(T).hash_code() == param.typeHash)
        {
            return std::any_cast<T>(param.data);
        }
        std::cerr << "[ERROR]\t"
                  << "Event param type is invalid: " << typeid(T).name()
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    EventId GetId() const
    {
        return mId;
    }

private:
    struct EventData
    {
        std::any data;
        size_t typeHash;
    };

    EventId mId { };
    std::unordered_map<ParamId, EventData> mData{};
};

