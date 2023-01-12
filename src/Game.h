#pragma once
#include "ComponentSystem/Components.h"
#include "ComponentSystem/EntityManager.h"
#include "Platform/Platform.hpp"
#include <catch2/catch.hpp>

class Game
{
private:
	//Window
	const float screentWidth = 800.0f;
	const float screenHeight = 600.0f;
	sf::RenderWindow* window;
	util::Platform platform;
	ComponentSystem::EntityManager manager;

	void Init();
	void InitPlayer();
	void PollingEvent();

public:
	Game();
	virtual ~Game();

	//Functions
	void Run();
	void Update();
	void Render();
};