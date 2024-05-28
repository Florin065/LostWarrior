#include "Components/RigidBody.hpp"
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
            auto& aTr = gCoordinator.GetComponent<Transform>(a);
            auto& bTr = gCoordinator.GetComponent<Transform>(b);
            auto& aCol = gCoordinator.GetComponent<Collider>(a);
            auto& bCol = gCoordinator.GetComponent<Collider>(b);

            if (aCol.layer == bCol.layer
            && aTr.__pos.x < bTr.__pos.x + bCol.length / 2
            && aTr.__pos.x + aCol.length / 2 > bTr.__pos.x
            && aTr.__pos.z < bTr.__pos.z + bCol.width / 2
            && aTr.__pos.z + aCol.width / 2 > bTr.__pos.z)
            {
                gCoordinator.LogInfo("x: ", aTr.__pos.x, " ", bTr.__pos.x);
                gCoordinator.LogInfo("z: ", aTr.__pos.z, " ", bTr.__pos.z);
                auto& aRbody = gCoordinator.GetComponent<RigidBody>(a);
                auto& bRbody = gCoordinator.GetComponent<RigidBody>(b);
                aRbody.velocity *= -1.0f;
                bRbody.velocity *= -1.0f;
                aTr.Translate(aRbody.velocity * dt);
                bTr.Translate(bRbody.velocity * dt);
            }
        }
    }
}

