#pragma once

#include <array>
#include <cassert>
#include <queue>

#include "Core/Types.hpp"


class EntityManager
{
public:
    EntityManager()
    {
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        {
            mAvailableEntities.push(entity);
        }
    }

    Entity CreateEntity()
    {
        assert(mLivingEntityCount < MAX_ENTITIES
            && "Too many entities in existence.");

        Entity id = mAvailableEntities.front();
        mAvailableEntities.pop();
        ++mLivingEntityCount;

        return id;
    }

    void DestroyEntity(Entity entity)
    {
        assert(entity < MAX_ENTITIES && "Cannot destroy entity: out of range.");

        mSignatures[entity].reset();
        mAvailableEntities.push(entity);
        --mLivingEntityCount;
    }

    void SetSignature(Entity entity, Signature signature)
    {
        assert(entity < MAX_ENTITIES && "Cannot set signature: out of range.");
    
        mSignatures[entity] = signature;
    }

    Signature GetSignature(Entity entity) const
    {
        assert(entity < MAX_ENTITIES && "Cannot get signature: out of range.");

        return mSignatures[entity];
    }

private:
    std::queue<Entity> mAvailableEntities{};
    std::array<Signature, MAX_ENTITIES> mSignatures{};
    Entity mLivingEntityCount{};
};

