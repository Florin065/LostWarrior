#pragma once

enum ColliderLayer {
    COLLIDER_NULL,
    COLLIDER_PHYSICAL,
    COLLIDERS_COUNT
};

struct Collider
{
    std::string name;
    ColliderLayer layer;
    float length;
    float width;
    float health = 1.0f;
    float damage = 1.0f;
    bool projectile = false;
    float ttl = 6.0f;
};

