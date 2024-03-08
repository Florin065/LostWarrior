#pragma once

#include <cstdint>
#include <bitset>

using Entity = std::uint64_t;
const Entity MAX_ENTITIES = 8192;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 64;

using Signature = std::bitset<MAX_COMPONENTS>;

