#pragma once

enum ColliderLayer {
    COLLIDER_NULL,
    COLLIDER_PHYSICAL,
    COLLIDERS_COUNT
};

struct Collider
{
    ColliderLayer layer;
    float length;
    float width;
};

