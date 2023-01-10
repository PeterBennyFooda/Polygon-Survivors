#include "GameEntity.h"

bool GameEntity::IsAlive()
{
	return alive;
}

void GameEntity::Destroy()
{
	alive = false;
}

void GameEntity::Update()
{
	for (auto& c : components)
	{
		c->Update();
	}
}

void GameEntity::Render()
{
	for (auto& c : components)
	{
		c->Render();
	}
}

template <typename T, typename... TArgs>
T& GameEntity::AddComponent(TArgs&&... mArgs)
{
	//allocate component of type 'T' on the HEAP
	//and forward the arguments to its constructor
	T* c(new T(std::forward<TArgs>(mArgs)...));

	//set the parent of the component to this instance
	c->entity = this;

	//wrap raw pointer to smart pointer
	std::unique_ptr<Component> uPtr(c);

	//add the smart pointer to the 'components' container
	//also, smart pointer is not copyable so we must move it
	components.emplace_back(std::move(uPtr));

	return *c;
}