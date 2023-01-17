#include "include/EntityFactory.h"
using namespace ComponentSystem;
using namespace std;

GameEntity& EntityFactory::CreatePlayer(const sf::Vector2f& position, sf::RenderWindow& target) noexcept
{
	auto& player(manager.AddEntity());

	player.AddComponent<CTransform>(position);

	auto& playerSprite(player.AddComponent<CSprite2D>(playerTexturePath, target));
	sf::Vector2f halfSize(playerSprite.Origin);

	player.AddComponent<CPhysics>(halfSize, ScreenWidth, ScreenHeight);
	player.AddComponent<CPlayerControl>(PlayerBaseSpeed);
	player.AddComponent<CParticle>(ScreenWidth, ScreenHeight, target);

	auto& playerStat(player.AddComponent<CStat>(3, 1));
	playerStat.CanBeProtect = true;
	player.AddGroup(EntityGroup::Player);

	return player;
}

GameEntity& EntityFactory::CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const int& health) noexcept
{
	return CreateEnemy(position, target, 1.f, EnemyMoveType::ChasePlayer, health);
}
GameEntity& EntityFactory::CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const float& speedMod, const int& health) noexcept
{
	return CreateEnemy(position, target, speedMod, EnemyMoveType::ChasePlayer, health);
}
GameEntity& EntityFactory::CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, EnemyMoveType moveType, const int& health) noexcept
{
	return CreateEnemy(position, target, 1.f, moveType, health);
}
GameEntity& EntityFactory::CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const float& speedMod, EnemyMoveType moveType, const int& health) noexcept
{
	auto& enemy(manager.AddEntity());

	enemy.AddComponent<CTransform>(position);

	string path = enemyTexturePath1;
	if (moveType == EnemyMoveType::AvoidPlayer)
		path = enemyTexturePath2;
	else if (moveType == EnemyMoveType::PingPong)
		path = enemyTexturePath3;

	auto& enemySprite(enemy.AddComponent<CSprite2D>(path, target));
	sf::Vector2f halfSize(enemySprite.Origin);

	enemy.AddComponent<CPhysics>(halfSize, ScreenWidth, ScreenHeight);
	enemy.AddComponent<CParticle>(ScreenWidth, ScreenHeight, target);

	auto& enemyStat(enemy.AddComponent<CStat>(health, speedMod));
	enemyStat.CanBeProtect = true;

	auto& players(manager.GetEntitiesByGroup(EntityGroup::Player));
	sf::Vector2f& playerPos(players[0]->GetComponent<CTransform>().Position);
	enemy.AddComponent<CSimpleEnemyControl>(EnemyBaseSpeed * enemyStat.SpeedMod, playerPos, moveType);

	enemy.AddGroup(EntityGroup::Enemy);

	return enemy;
}

ComponentSystem::GameEntity& EntityFactory::CreateProjectile(const sf::Vector2f& position, const sf::Vector2f& direction,
	sf::RenderWindow& target, const float& speedMod, const int& damage) noexcept
{
	auto& projectile(manager.AddEntity());

	auto& projectileTransform(projectile.AddComponent<CTransform>(position));
	projectileTransform.Size = sf::Vector2f(0.25f, 0.25f);

	auto& projectileSprite(projectile.AddComponent<CSprite2D>(playerTexturePath, target));
	sf::Vector2f halfSize(projectileSprite.Origin);

	projectile.AddComponent<CPhysics>(halfSize, ScreenWidth, ScreenHeight);
	projectile.AddComponent<CProjectile>(BulletBaseSpeed * speedMod, direction, damage);

	projectile.AddGroup(EntityGroup::Projectile);

	return projectile;
}

ComponentSystem::GameEntity& EntityFactory::CreateObstacle(const sf::Vector2f& position, sf::RenderWindow& target) noexcept
{
	auto& obstacle(manager.AddEntity());

	obstacle.AddComponent<CTransform>(position);

	auto& obstacleSprite(obstacle.AddComponent<CSprite2D>(rockTexturePath, target));
	sf::Vector2f halfSize(obstacleSprite.Origin);

	obstacle.AddComponent<CPhysics>(halfSize, ScreenWidth, ScreenHeight);
	obstacle.AddGroup(EntityGroup::Obstacle);

	return obstacle;
}