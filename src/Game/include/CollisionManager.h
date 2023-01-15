#pragma once
#include "ComponentSystem/EntityManager.h"
#include "Components.h"
#include "GlobalEventCenter.h"
#include "GlobalGameSettings.h"

class CollisionManager
{
private:
	ComponentSystem::EntityManager& manager;

	template <class T1, class T2>
	bool IsIntersecting(T1& mA, T2& mB) noexcept;
	bool stop { false };

public:
	CollisionManager(ComponentSystem::EntityManager& mManager);

	void TestAllCollision();
	void TestCollision(ComponentSystem::GameEntity& a, ComponentSystem::GameEntity& b) noexcept;
};

template <class T1, class T2>
bool CollisionManager::IsIntersecting(T1& mA, T2& mB) noexcept
{
	return mA.Right() >= mB.Left() && mA.Left() <= mB.Right() && mA.Bottom() >= mB.Top() && mA.Top() <= mB.Bottom();
}
