#include "include/CollisionManager.h"
using namespace std;
using namespace ComponentSystem;

CollisionManager::CollisionManager(ComponentSystem::EntityManager& mManager, eventpp::EventDispatcher<int, void(int)>& mDispatcher) :
	manager(mManager),
	gameDispatcher(mDispatcher)
{
	gameDispatcher.appendListener(EventNames::GameStart, [this](int n) {
		UNUSED(n);
		stop = false;
	});
	gameDispatcher.appendListener(EventNames::Win, [this](int n) {
		UNUSED(n);
		stop = true;
	});
	gameDispatcher.appendListener(EventNames::GameOver, [this](int n) {
		UNUSED(n);
		stop = true;
	});
}

void CollisionManager::TestCollision(GameEntity& a, GameEntity& b) noexcept
{
	if (stop)
		return;

	auto& pA(a.GetComponent<CPhysics>());
	auto& pB(b.GetComponent<CPhysics>());

	if (IsIntersecting(pA, pB))
	{
		if (a.HasGroup(EntityGroup::Enemy))
		{
			auto& cA(a.GetComponent<CSimpleEnemyControl>());
			auto& statA(a.GetComponent<CStat>());
			cA.Stop = true;

			if (b.HasGroup(EntityGroup::Player) && !statA.IsDead)
			{
				auto& statB(b.GetComponent<CStat>());
				auto& cB(b.GetComponent<CPlayerControl>());

				if (!statB.IsInvincible)
					gameDispatcher.dispatch(EventNames::PlayerHPChange, -1);
				statB.Hit(1);

				if (statB.IsDead)
				{
					gameDispatcher.dispatch(EventNames::GameOver);
					cB.Stop = true;
				}
			}
		}

		if (b.HasGroup(EntityGroup::Enemy))
		{
			auto& cB(b.GetComponent<CSimpleEnemyControl>());
			auto& statB(b.GetComponent<CStat>());
			cB.Stop = true;

			if (a.HasGroup(EntityGroup::Player) && !statB.IsDead)
			{
				auto& statA(a.GetComponent<CStat>());
				auto& cA(a.GetComponent<CPlayerControl>());

				if (!statA.IsInvincible)
					gameDispatcher.dispatch(EventNames::PlayerHPChange, -1);
				statA.Hit(1);

				if (statA.IsDead)
				{
					gameDispatcher.dispatch(EventNames::GameOver);
					cA.Stop = true;
				}
			}
		}

		// if (a.HasGroup(EntityGroup::Player) && b.HasGroup(EntityGroup::Obstacle))
		// {
		// 	auto& cA(a.GetComponent<CPlayerControl>());
		// 	cA.Stop = true;
		// }
		// else if (b.HasGroup(EntityGroup::Player) && a.HasGroup(EntityGroup::Obstacle))
		// {
		// 	auto& cB(b.GetComponent<CPlayerControl>());
		// 	cB.Stop = true;
		// }

		if (a.HasGroup(EntityGroup::Projectile) && b.HasGroup(EntityGroup::Enemy))
		{
			auto& statB(b.GetComponent<CStat>());
			auto& pjA(a.GetComponent<CProjectile>());

			if (statB.IsDead)
				gameDispatcher.dispatch(EventNames::ScoreChange, statB.GetScore());
			else
			{
				statB.Hit(pjA.Damage);
				a.Destroy();
			}
		}
		else if (b.HasGroup(EntityGroup::Projectile) && a.HasGroup(EntityGroup::Enemy))
		{
			auto& statA(a.GetComponent<CStat>());
			auto& pjB(b.GetComponent<CProjectile>());

			if (statA.IsDead)
				gameDispatcher.dispatch(EventNames::ScoreChange, statA.GetScore());
			else
			{
				statA.Hit(pjB.Damage);
				b.Destroy();
			}
		}
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
	if (stop)
		return;

	auto& players(manager.GetEntitiesByGroup(EntityGroup::Player));
	auto& enemies(manager.GetEntitiesByGroup(EntityGroup::Enemy));
	//auto& obstacles(manager.GetEntitiesByGroup(EntityGroup::Obstacle));
	auto& projectiles(manager.GetEntitiesByGroup(EntityGroup::Projectile));

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
	// for (size_t i = 0; i < players.size(); i++)
	// {
	// 	auto& p(players[i]);
	// 	//Check collisions with all obstacles.
	// 	for (size_t j = 0; j < obstacles.size(); j++)
	// 	{
	// 		auto& o(obstacles[j]);
	// 		TestCollision(*p, *o);
	// 	}
	// }

	//Projectiles only collide with enemies.
	for (size_t i = 0; i < projectiles.size(); i++)
	{
		auto& pj(projectiles[i]);
		//Check collisions with all enemies.
		for (size_t j = 0; j < enemies.size(); j++)
		{
			auto& e(enemies[j]);
			TestCollision(*pj, *e);
		}
	}
}