#pragma once
#include "ComponentSystemDefine.h"
#include "GameEntity.h"

/////////////////////////////////////////////////
///This file is for EntityManager that handles
///all GameEntity and their states.
/////////////////////////////////////////////////
namespace ComponentSystem
{
class GameEntity;

class EntityManager
{
private:
	std::vector<std::unique_ptr<GameEntity>> entities;
	std::array<std::vector<GameEntity*>, MaxGroups> groupedEntities;

public:
	void Update(float mFT);
	void Render();
	void Refresh();

	GameEntity& AddEntity();

	void AddToGroup(GameEntity* entity, Group group);
	std::vector<GameEntity*>& GetEntitiesByGroup(Group group);
};

}
