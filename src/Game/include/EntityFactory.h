#pragma once
#include "ComponentSystem/EntityManager.h"
#include "Components.h"
#include "GlobalGameSettings.h"

class EntityFactory
{
private:
	ComponentSystem::EntityManager& manager;

public:
	EntityFactory(ComponentSystem::EntityManager& mManager) :
		manager(mManager)
	{}

	ComponentSystem::GameEntity& CreatePlayer(const sf::Vector2f& position, sf::RenderWindow& target) noexcept;

	ComponentSystem::GameEntity& CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target) noexcept;
	ComponentSystem::GameEntity& CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const float& speedMod) noexcept;
	ComponentSystem::GameEntity& CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, EnemyMoveType moveType) noexcept;
	ComponentSystem::GameEntity& CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const float& speedMod, EnemyMoveType moveType) noexcept;

	ComponentSystem::GameEntity& CreateProjectile(const sf::Vector2f& position, const sf::Vector2f& direction,
		sf::RenderWindow& target, const float& speedMod) noexcept;

	ComponentSystem::GameEntity& CreateObstacle(const sf::Vector2f& position, sf::RenderWindow& target) noexcept;
};
