#pragma once
#include "GameEntity.h"

class EntityManager
{
private:
	std::vector<std::unique_ptr<GameEntity>> entities;

public:
	void Update();
	void Render();

	GameEntity& AddEntity();
};
