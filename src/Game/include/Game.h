#pragma once
#include "CollisionManager.h"
#include "ComponentSystem/EntityManager.h"
#include "Components.h"
#include "EnemySpawner.h"
#include "EntityFactory.h"
#include "GameClock.h"
#include "GlobalEventCenter.h"
#include "GlobalGameSettings.h"
#include "Platform/Platform.hpp"
#include "WeaponController.h"
#include "eventpp/eventdispatcher.h"
#include <catch2/catch.hpp>
#include <chrono>
#include <omp.h>

/////////////////////////////////////////////////
///
///This file handles the main game logic.
///
/////////////////////////////////////////////////
class Game
{
private:
	const float ftSlice { 0.02f }; //The time slice length we want to update our game logic one time in.
	const float ftStep { 0.02f };  //Should be the same as 'ftSlice'. The time actually passed to the game logic, making sure we have constant result.
	const int maxLoop { 200 };	   //Avoid updating too many time when we have really high FPS.
	float lastFrameTime { 0.f };
	float currentSlice { 0.f };

	std::chrono::steady_clock::time_point timePoint1;
	std::chrono::steady_clock::time_point timePoint2;

	float frameTimeSeconds;
	float framePerSecond;

	sf::RenderWindow* window;
	util::Platform platform;

	ComponentSystem::EntityManager manager;
	CollisionManager* collisionManager { nullptr };
	EntityFactory* entityFactory { nullptr };
	WeaponController* playerWeapon { nullptr };
	EnemySpawner* enemySpawner { nullptr };

	GameClock* gameClock { nullptr };
	float currentSpawnCount { 5 };
	bool spawnLock { false };
	EnemySpawnMode currentWaveMode { EnemySpawnMode::Easy };

	int currentScore { 0 };

	void Init();
	void InitLevel();
	void InitPlayer();
	void InitEnemy();
	void GenerateLevel();
	void GenerateEnemyWave();
	void PollingEvent();
	void OnGameStateChange(EventNames state);

public:
	GameStates GameState;

	Game();
	virtual ~Game();

	//Functions
	void Run();
	void FixedUpdate();
	void Update();
	void Render();
};