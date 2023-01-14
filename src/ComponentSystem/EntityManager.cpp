#include "EntityManager.h"

namespace ComponentSystem
{

void EntityManager::Update(float mFT)
{
	for (auto& e : entities)
		e->Update(mFT);
}

void EntityManager::Render()
{
	for (auto& e : entities)
		e->Render();
}

void EntityManager::Refresh()
{
	//Remove dead entities or entities not in the group
	//from the current group.
	for (auto i(0u); i < MaxGroups; ++i)
	{
		auto& entitiesInGroup(groupedEntities[i]);

		entitiesInGroup.erase(std::remove_if(std::begin(entitiesInGroup), std::end(entitiesInGroup), [i](GameEntity* mEntity) {
			return !mEntity->IsAlive() || !mEntity->HasGroup(i);
		}),
			std::end(entitiesInGroup));
	}

	//Remove dead entities.
	entities.erase(
		std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<GameEntity>& mEntity) {
			return !mEntity->IsAlive();
		}),
		std::end(entities));
}

GameEntity& EntityManager::AddEntity()
{
	GameEntity* e(new GameEntity(*this));
	std::unique_ptr<GameEntity> uPtr { e };
	entities.emplace_back(std::move(uPtr));
	return *e;
}

void EntityManager::AddToGroup(GameEntity* entity, Group group)
{
	groupedEntities[group].emplace_back(entity);
}

std::vector<GameEntity*>& EntityManager::GetEntitiesByGroup(Group group)
{
	return groupedEntities[group];
}

}
