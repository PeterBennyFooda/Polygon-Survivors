#include "EntityFactory.h"
using namespace ComponentSystem;
using namespace std;

GameEntity& EntityFactory::CreatePlayer(const sf::Vector2f& position, sf::RenderWindow* target) noexcept
{
	auto& player(manager.AddEntity());

	player.AddComponent<CTransform>(position);
	auto& playerSprite(player.AddComponent<CSprite2D>(playerTexturePath, target));
	sf::Vector2f halfSize(playerSprite.Sprite.getOrigin());
	player.AddComponent<CPhysics>(halfSize, ScreentWidth, ScreenHeight);
	player.AddComponent<CPlayerControl>(PlayerBaseSpeed);

	player.AddGroup(CharacterGroup::Player);

	return player;
}

GameEntity& EntityFactory::CreateEnemy(const sf::Vector2f& position, sf::RenderWindow* target) noexcept
{
	auto& enemy(manager.AddEntity());

	enemy.AddComponent<CTransform>(position);
	auto& enemySprite(enemy.AddComponent<CSprite2D>(enemyTexturePath, target));
	sf::Vector2f halfSize(enemySprite.Sprite.getOrigin());
	enemy.AddComponent<CPhysics>(halfSize, ScreentWidth, ScreenHeight);

	auto& players(manager.GetEntityByGroup(CharacterGroup::Player));
	sf::Vector2f& playerPos(players[0]->GetComponent<CTransform>().Position);
	enemy.AddComponent<CSimpleEnemyControl>(EnemyBaseSpeed, playerPos);

	enemy.AddGroup(CharacterGroup::Enemy);

	return enemy;
}