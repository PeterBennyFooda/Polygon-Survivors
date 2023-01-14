#include "CollisionManager.h"
#include "GlobalGameSettings.h"

using namespace ComponentSystem;

void CollisionManager::TestCollision(GameEntity& a, GameEntity& b) noexcept
{
	auto& pA(a.GetComponent<CPhysics>());
	auto& pB(b.GetComponent<CPhysics>());

	if (IsIntersecting(pA, pB))
	{
		if (a.HasGroup(EntityGroup::Enemy))
		{
			auto& cA(a.GetComponent<CSimpleEnemyControl>());
			cA.Stop = true;
		}

		if (b.HasGroup(EntityGroup::Enemy))
		{
			auto& cB(b.GetComponent<CSimpleEnemyControl>());
			cB.Stop = true;
		}

		// if (a.HasGroup(EntityGroup::Player) && b.HasGroup(EntityGroup::Obstacle))
		// {
		// 	// auto& cA(a.GetComponent<CPlayerControl>());
		// 	// cA.Stop = true;
		// }
		// else if (b.HasGroup(EntityGroup::Player) && a.HasGroup(EntityGroup::Obstacle))
		// {
		// 	auto& cB(b.GetComponent<CPlayerControl>());
		// 	cB.Stop = true;
		// }
	}
	else
	{
		if (a.HasGroup(EntityGroup::Enemy))
		{
			auto& cA(a.GetComponent<CSimpleEnemyControl>());
			cA.Stop = false;
		}

		if (b.HasGroup(EntityGroup::Enemy))
		{
			auto& cB(b.GetComponent<CSimpleEnemyControl>());
			cB.Stop = false;
		}

		// if (a.HasGroup(EntityGroup::Player) && b.HasGroup(EntityGroup::Obstacle))
		// {
		// 	auto& cA(a.GetComponent<CPlayerControl>());
		// 	cA.Stop = false;
		// }
		// else if (b.HasGroup(EntityGroup::Player) && a.HasGroup(EntityGroup::Obstacle))
		// {
		// 	auto& cB(b.GetComponent<CPlayerControl>());
		// 	cB.Stop = false;
		// }
	}
}

void CollisionManager::TestAllCollision()
{
	auto& players(manager.GetEntitiesByGroup(EntityGroup::Player));
	auto& enemies(manager.GetEntitiesByGroup(EntityGroup::Enemy));
	auto& obstacles(manager.GetEntitiesByGroup(EntityGroup::Obstacle));

	//Enemies only collide with players.
	for (size_t i = 0; i < enemies.size(); i++)
	{
		auto& e1(enemies[i]);
		//Check collisions with all players.
		for (size_t j = 0; j < players.size(); j++)
		{
			auto& p(players[j]);
			TestCollision(*e1, *p);
		}
	}

	//Players only collide with obstacles.
	for (size_t i = 0; i < players.size(); i++)
	{
		auto& p(players[i]);
		//Check collisions with all obstacles.
		for (size_t j = 0; j < obstacles.size(); j++)
		{
			auto& o(obstacles[j]);
			TestCollision(*p, *o);
		}
	}
}