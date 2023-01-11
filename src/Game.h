#pragma once
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