#pragma once

#include <memory>

#include "Core/Io/LogManager.hpp"
#include "Core/Event/EventManager.hpp"

#include "Core/EntityManager.hpp"
#include "Core/ComponentManager.hpp"
#include "Core/SystemManager.hpp"

class Coordinator
{
public:
    Coordinator(LogLevel logLevel)
        : mLogManager(std::make_unique<LogManager>(logLevel)),
          mEventManager(std::make_unique<EventManager>()),
          mEntityManager(std::make_unique<EntityManager>()),
          mComponentManager(std::make_unique<ComponentManager>()),
          mSystemManager(std::make_unique<SystemManager>())
    { }

    // EventManager Methods
    void AddListener(EventId id, std::function<void(Event const&)> const& listener) const
    {
        mEventManager.get()->AddListener(id, listener);
    }

    void SendEvent(Event const& event) const
    {
        mEventManager.get()->SendEvent(event);
    }

    void SendEvent(const EventId id) const
    {
        mEventManager.get()->SendEvent(id);
    }

    // EntityManager Methods
    Entity CreateEntity() const
    {
        return mEntityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity) const
    {
        mEntityManager->DestroyEntity(entity);
        mComponentManager->EntityDestroyed(entity);
        mSystemManager->EntityDestroyed(entity);
    }

    // ComponentManager Methods
    template<typename T>
    void RegisterComponent() const
    {
        mComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component) const
    {
        mComponentManager->AddComponent(entity, component);

        auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
	void RemoveComponent(Entity entity) const
	{
		mComponentManager->RemoveComponent<T>(entity);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

    template<typename T>
	T& GetComponent(Entity entity) const
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType() const
	{
		return mComponentManager->GetComponentType<T>();
	}

    // SystemManager Methods
    template<typename T>
	std::shared_ptr<T> RegisterSystem() const
	{
		return mSystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature) const
	{
		mSystemManager->SetSignature<T>(signature);
	}

    // LogManager Methods
    template <typename... Args>
    void LogDebug(Args&&... args) const
    {
        mLogManager->Debug(args...);
    }

    template <typename... Args>
    void LogInfo(Args&&... args) const
    {
        mLogManager->Info(args...);
    }

    template <typename... Args>
    void LogError(Args&&... args) const
    {
        mLogManager->Error(args...);
    }

    template<typename T, typename... Args>
    void Assert(T condition, Args&&... args) const
    {
        mLogManager->Assert(condition, args...);
    }

private:
    const std::unique_ptr<LogManager> mLogManager;
    const std::unique_ptr<EventManager> mEventManager;
    const std::unique_ptr<EntityManager> mEntityManager;
    const std::unique_ptr<ComponentManager> mComponentManager;
    const std::unique_ptr<SystemManager> mSystemManager;
};

