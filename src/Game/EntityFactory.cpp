#include "EntityFactory.h"
using namespace ComponentSystem;
using namespace std;

GameEntity& EntityFactory::CreatePlayer(const sf::Vector2f& position, sf::RenderWindow& target) noexcept
{
	auto& player(manager.AddEntity());

	player.AddComponent<CTransform>(position);

	auto& playerSprite(player.AddComponent<CSprite2D>(playerTexturePath, target));
	sf::Vector2f halfSize(playerSprite.Sprite.getOrigin());

	player.AddComponent<CPhysics>(halfSize, ScreentWidth, ScreenHeight);
	player.AddComponent<CPlayerControl>(PlayerBaseSpeed);

	player.AddGroup(EntityGroup::Player);

	return player;
}

GameEntity& EntityFactory::CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target) noexcept
{
	return CreateEnemy(position, target, 1.f, EnemyMoveType::ChasePlayer);
}
GameEntity& EntityFactory::CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const float& speedMod) noexcept
{
	return CreateEnemy(position, target, speedMod, EnemyMoveType::ChasePlayer);
}
GameEntity& EntityFactory::CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, EnemyMoveType moveType) noexcept
{
	return CreateEnemy(position, target, 1.f, moveType);
}
GameEntity& EntityFactory::CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const float& speedMod, EnemyMoveType moveType) noexcept
{
	auto& enemy(manager.AddEntity());

	enemy.AddComponent<CTransform>(position);

	auto& enemySprite(enemy.AddComponent<CSprite2D>(enemyTexturePath, target));
	sf::Vector2f halfSize(enemySprite.Sprite.getOrigin());

	enemy.AddComponent<CPhysics>(halfSize, ScreentWidth, ScreenHeight);
	auto& players(manager.GetEntitiesByGroup(EntityGroup::Player));
	sf::Vector2f& playerPos(players[0]->GetComponent<CTransform>().Position);
	enemy.AddComponent<CSimpleEnemyControl>(EnemyBaseSpeed * speedMod, playerPos, moveType);

	enemy.AddGroup(EntityGroup::Enemy);

	return enemy;
}

ComponentSystem::GameEntity& EntityFactory::CreateProjectile(const sf::Vector2f& position, const sf::Vector2f& direction,
	sf::RenderWindow& target, const float& speedMod) noexcept
{
	auto& projectile(manager.AddEntity());

	auto& projectileTransform(projectile.AddComponent<CTransform>(position));
	projectileTransform.Size = sf::Vector2f(0.25f, 0.25f);

	auto& projectileSprite(projectile.AddComponent<CSprite2D>(playerTexturePath, target));
	sf::Vector2f halfSize(projectileSprite.Sprite.getOrigin());

	projectile.AddComponent<CPhysics>(halfSize, ScreentWidth, ScreenHeight);
	projectile.AddComponent<CProjectile>(BulletBaseSpeed * speedMod, direction);

	projectile.AddGroup(EntityGroup::Projectile);

	return projectile;
}

ComponentSystem::GameEntity& EntityFactory::CreateObstacle(const sf::Vector2f& position, sf::RenderWindow& target) noexcept
{
	auto& obstacle(manager.AddEntity());

	obstacle.AddComponent<CTransform>(position);

	auto& obstacleSprite(obstacle.AddComponent<CSprite2D>(rockTexturePath, target));
	sf::Vector2f halfSize(obstacleSprite.Sprite.getOrigin());

	obstacle.AddComponent<CPhysics>(halfSize, ScreentWidth, ScreenHeight);
	obstacle.AddGroup(EntityGroup::Obstacle);

	return obstacle;
}