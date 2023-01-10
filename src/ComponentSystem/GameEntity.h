#pragma once
#include <memory>

class GameEntity;

struct Component
{
	GameEntity* entity { nullptr }; // use this to store parent entity

	virtual ~Component()
	{}

	virtual void Update()
	{}
	virtual void Render()
	{}
};

class GameEntity
{
private:
	bool alive { true };
	std::vector<std::unique_ptr<Component>> components; //for polymorphism

public:
	bool IsAlive();
	void Destroy();

	void Update();
	void Render();

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs);
};