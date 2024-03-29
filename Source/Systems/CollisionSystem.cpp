#include "glad/gl.h"

#include "Core/Coordinator.hpp"

#include "Components/Transform.hpp"
#include "Components/Collider.hpp"

#include "Systems/CollisionSystem.hpp"


extern Coordinator gCoordinator;


void CollisionSystem::Init()
{

}

void CollisionSystem::Shutdown()
{

}

void CollisionSystem::Update([[maybe_unused]] float dt)
{
    std::set<Entity>::iterator it;
    std::set<Entity>::iterator jt;

    for (it = mEntities.begin(); it != mEntities.end(); ++it)
    {
        for (jt = std::next(it, 1); jt != mEntities.end(); ++jt)
        {
            Entity a = *it;
            Entity b = *jt;

            auto& aTransform = gCoordinator.GetComponent<Transform>(a);
            auto& aCollider  = gCoordinator.GetComponent<Collider>(a);
            auto& bTransform = gCoordinator.GetComponent<Transform>(b);
            auto& bCollider  = gCoordinator.GetComponent<Collider>(b);

            if (glm::distance(aTransform.__pos, bTransform.__pos)
              < aCollider.radius + bCollider.radius)
            {

            }
        }
    }
}

