#pragma once
#include "ComponentSystem/EntityManager.h"
#include "Components.h"
#include "GlobalGameSettings.h"
#include "eventpp/eventdispatcher.h"

class CollisionManager
{
private:
	ComponentSystem::EntityManager& manager;
	eventpp::EventDispatcher<int, void(int)>& gameDispatcher;

	template <class T1, class T2>
	bool IsIntersecting(T1& mA, T2& mB) noexcept;
	bool stop { false };

public:
	CollisionManager(ComponentSystem::EntityManager& mManager, eventpp::EventDispatcher<int, void(int)>& dispatcher);

	void TestAllCollision();
	void TestCollision(ComponentSystem::GameEntity& a, ComponentSystem::GameEntity& b) noexcept;
};

template <class T1, class T2>
bool CollisionManager::IsIntersecting(T1& mA, T2& mB) noexcept
{
	return mA.Right() >= mB.Left() && mA.Left() <= mB.Right() && mA.Bottom() >= mB.Top() && mA.Top() <= mB.Bottom();
}
