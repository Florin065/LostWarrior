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

        transform.Translate(rigidBody.velocity * dt);
        rigidBody.velocity += rigidBody.mass * rigidBody.force * dt;

        transform.Rotate(rigidBody.angularVel * dt);
    }
}
