#include "EntityManager.h"

void EntityManager::Update()
{
	entities.erase(
		std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<GameEntity>& mEntity) {
			return !mEntity->IsAlive();
		}),
		std::end(entities));

	for (auto& e : entities)
		e->Update();
}

void EntityManager::Render()
{
	for (auto& e : entities)
		e->Render();
}

GameEntity& EntityManager::AddEntity()
{
	GameEntity* e { new GameEntity {} };
	std::unique_ptr<GameEntity> uPtr { e };
	entities.emplace_back(std::move(uPtr));
	return *e;
}