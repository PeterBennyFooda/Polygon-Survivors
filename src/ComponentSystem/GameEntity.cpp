#include "GameEntity.h"

namespace ComponentSystem
{

bool GameEntity::IsAlive() const
{
	return alive;
}

void GameEntity::Destroy()
{
	alive = false;
}

void GameEntity::Update(float mFT)
{
	for (auto& c : components)
	{
		c->Update(mFT);
	}
}

void GameEntity::Render()
{
	for (auto& c : components)
	{
		c->Render();
	}
}

bool GameEntity::HasGroup(Group group) const noexcept
{
	return groupBitset[group];
}

void GameEntity::AddGroup(Group group) noexcept
{
	groupBitset[group] = true;
	manager.AddToGroup(this, group);
}

void GameEntity::DeleteGroup(Group group) noexcept
{
	groupBitset[group] = false;
}

}
