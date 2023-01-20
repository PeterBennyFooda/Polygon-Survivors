#pragma once
#include "ComponentSystem/EntityManager.h"
#include "Components.h"
#include "GlobalGameSettings.h"
#include "eventpp/eventdispatcher.h"

class EntityFactory
{
private:
	ComponentSystem::EntityManager& manager;
	eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies>& gameDispatcher;

public:
	EntityFactory(ComponentSystem::EntityManager& mManager,
		eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies>& mDispatcher) :
		manager(mManager),
		gameDispatcher(mDispatcher)
	{}

	ComponentSystem::GameEntity& CreatePlayer(const sf::Vector2f& position, sf::RenderWindow& target) noexcept;

	ComponentSystem::GameEntity& CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const int& health) noexcept;
	ComponentSystem::GameEntity& CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const float& speedMod, const int& health) noexcept;
	ComponentSystem::GameEntity& CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, EnemyMoveType moveType, const int& health) noexcept;
	ComponentSystem::GameEntity& CreateEnemy(const sf::Vector2f& position, sf::RenderWindow& target, const float& speedMod, EnemyMoveType moveType, const int& health) noexcept;

	ComponentSystem::GameEntity& CreateProjectile(const sf::Vector2f& position, const sf::Vector2f& direction,
		sf::RenderWindow& target, const float& speedMod, const int& damage) noexcept;

	ComponentSystem::GameEntity& CreateObstacle(const sf::Vector2f& position, sf::RenderWindow& target) noexcept;
};
