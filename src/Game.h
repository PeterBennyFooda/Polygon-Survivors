#pragma once
#include "Platform/Platform.hpp"
#include "Player.h"

class Game
{
private:
	//Window
	sf::RenderWindow* window;
	util::Platform platform;
	const float screentWidth = 800.0f;
	const float screenHeight = 600.0f;

	//Player
	Player* player;

	void Init();
	void InitPlayer();

public:
	Game();
	virtual ~Game();

	//Functions
	void Run();
	void Update();
	void Render();
};