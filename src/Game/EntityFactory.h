#pragma once
#include "ComponentSystem/Components.h"
#include "ComponentSystem/EntityManager.h"
#include "GlobalGameSettings.h"

class EntityFactory
{
private:
	const std::string playerTexturePath = "Resources/Texture/Character/player.png";
	const std::string enemyTexturePath = "Resources/Texture/Character/enemy.png";

	ComponentSystem::EntityManager& manager;

public:
	EntityFactory(ComponentSystem::EntityManager& mManager) :
		manager(mManager)
	{}

	enum CharacterGroup : std::size_t
	{
		Player,
		Enemy,
		Obstacle
	};

	ComponentSystem::GameEntity& CreatePlayer(const sf::Vector2f& position, sf::RenderWindow* target) noexcept;
	ComponentSystem::GameEntity& CreateEnemy(const sf::Vector2f& position, sf::RenderWindow* target) noexcept;
};
