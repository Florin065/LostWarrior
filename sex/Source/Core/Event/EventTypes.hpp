#pragma once

#include <cstdint>


// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
	return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
	return fnv1a_32(s, count);
}

typedef std::uint32_t EventId;
typedef std::uint32_t ParamId;

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

#define EVENT_DEFINE_START(ns) \
    namespace Events::ns { \
        [[maybe_unused]] const EventId ID = "Events::"#ns"::ID"##_hash;

#define EVENT_PARAM_DEFINE(ns, id) \
        [[maybe_unused]] const ParamId id = "Events::"#ns"::"#id##_hash;

#define EVENT_DEFINE_END \
    }

EVENT_DEFINE_START(Window::Resize)
EVENT_PARAM_DEFINE(Window::Resize, WIDTH)
EVENT_PARAM_DEFINE(Window::Resize, HEIGHT)
EVENT_DEFINE_END

EVENT_DEFINE_START(Input::Async::Key)
EVENT_PARAM_DEFINE(Input::Async::Key, KEY)
EVENT_PARAM_DEFINE(Input::Async::Key, SCANCODE)
EVENT_PARAM_DEFINE(Input::Async::Key, ACTION)
EVENT_PARAM_DEFINE(Input::Async::Key, MODS)
EVENT_DEFINE_END

EVENT_DEFINE_START(Input::Cursor)
EVENT_PARAM_DEFINE(Input::Cursor, XPOS)
EVENT_PARAM_DEFINE(Input::Cursor, YPOS)
EVENT_PARAM_DEFINE(Input::Cursor, DX)
EVENT_PARAM_DEFINE(Input::Cursor, DY)
EVENT_DEFINE_END


EVENT_DEFINE_START(Input::Sync::Key)
EVENT_PARAM_DEFINE(Input::Sync::Key, KEYS)
EVENT_DEFINE_END

