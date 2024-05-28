#include "Components/RigidBody.hpp"
#include "Core/ResourceManager.hpp"
#include "glad/gl.h"

#include "Core/Coordinator.hpp"

#include "Components/Transform.hpp"
#include "Components/Collider.hpp"

#include "Systems/CollisionSystem.hpp"
#include "MapGenerationSystem.hpp"


extern Coordinator gCoordinator;
extern ResourceManager gResourceManager;
extern int completion;
extern std::vector<Entity> doors;
extern std::vector<Entity> entities;
extern Entity player;


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

    std::vector<Entity> markedForDeletion;

    for (it = mEntities.begin(); it != mEntities.end(); ++it)
    {
        Entity possiblyAnArrow = *it;
        auto& col = gCoordinator.GetComponent<Collider>(possiblyAnArrow);
        if (col.projectile)
        {
            if (col.ttl <= 0.0f)
            {
                markedForDeletion.push_back(possiblyAnArrow);
            }
            else
            {
                col.ttl -= dt;
            }
        }
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
            && aTr.__pos.z + aCol.width / 2 > bTr.__pos.z
            && !(aCol.projectile && bCol.projectile)
            && !(aCol.projectile && bCol.name == "player")
            && !(bCol.projectile && aCol.name == "player"))
            {
                if (aCol.name == "open_door" || bCol.name == "open_door")
                {
                    doors.clear();
                    for (auto& entity : entities)
                    {
                        gCoordinator.DestroyEntity(entity);
                    }
                    entities.clear();
                    completion++;
                    gResourceManager.GetShader("default").get()->SetUniform("completion", completion);
                    generateEntitiesFromMap(gCoordinator, gResourceManager, completion < 10 ? 1 : 2, entities, player, completion, doors);
                    return;
                    
                }
                gCoordinator.LogDebug(aCol.name, ": ", aTr.__pos.x, " ", aTr.__pos.z);
                gCoordinator.LogDebug(bCol.name, ": ", bTr.__pos.x, " ", bTr.__pos.z);
                auto& aRbody = gCoordinator.GetComponent<RigidBody>(a);
                auto& bRbody = gCoordinator.GetComponent<RigidBody>(b);
                aRbody.velocity *= -1.0f;
                bRbody.velocity *= -1.0f;
                aTr.Translate(aRbody.velocity * dt);
                bTr.Translate(bRbody.velocity * dt);
                aCol.health -= bCol.damage;
                bCol.health -= aCol.damage;
                if (aCol.health <= 0.0f)
                {
                    markedForDeletion.push_back(a);
                }
                if (bCol.health <= 0.0f)
                {
                    markedForDeletion.push_back(b);
                }
            }
        }
    }

    for (auto& entity : markedForDeletion)
    {
        Event e(Events::Game::KilledEnemy::ID);
        e.SetParam(Events::Game::KilledEnemy::ENTITY_ID, entity);
        gCoordinator.SendEvent(e);
        gCoordinator.DestroyEntity(entity);
    }
}

