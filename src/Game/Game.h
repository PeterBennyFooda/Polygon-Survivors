#pragma once
#include "ComponentSystem/Components.h"
#include "ComponentSystem/EntityManager.h"
#include "Platform/Platform.hpp"
#include <catch2/catch.hpp>
#include <chrono>

/////////////////////////////////////////////////
///
///This file handles the main game logic.
///
/////////////////////////////////////////////////
class Game
{
private:
	//Window
	const float screentWidth = 800.0f;
	const float screenHeight = 600.0f;

	const float ftSlice { 0.06f }; //The time slice length we want to update our game logic one time in.
	const float ftStep { 0.06f };  //Should be the same as 'ftSlice'. The time actually passed to the game logic, making sure we have constant result.
	float lastFrameTime { 0.f };
	float currentSlice { 0.f };

	std::chrono::steady_clock::time_point timePoint1;
	std::chrono::steady_clock::time_point timePoint2;

	float frameTimeSeconds;
	float framePerSecond;

	sf::RenderWindow* window;
	util::Platform platform;
	ComponentSystem::EntityManager manager;

	void Init();
	void InitLevel();
	void InitPlayer();
	void InitEnemy();
	void PollingEvent();

public:
	Game();
	virtual ~Game();

	//Functions
	void Run();
	void FixedUpdate();
	void Update();
	void Render();
};