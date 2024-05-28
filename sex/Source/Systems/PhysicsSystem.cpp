#include "Core/Coordinator.hpp"

#include "Components/Transform.hpp"
#include "Components/RigidBody.hpp"

#include "Systems/PhysicsSystem.hpp"


extern Coordinator gCoordinator;


void PhysicsSystem::Init()
{

}

void PhysicsSystem::Shutdown()
{

}

void PhysicsSystem::Update(float dt)
{
    for (auto const& entity : mEntities)
    {
        auto& transform = gCoordinator.GetComponent<Transform>(entity);   
        auto& rigidBody = gCoordinator.GetComponent<RigidBody>(entity);

        for (auto const& [_, force] : rigidBody.forces)
        {
            rigidBody.velocity += rigidBody.mass * force * dt;
        }
        rigidBody.velocity -= rigidBody.drag * rigidBody.velocity;
        transform.Translate(rigidBody.velocity * dt);

        transform.Rotate(rigidBody.angularVel * dt);
    }
}

