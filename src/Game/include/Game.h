#pragma once
#include "CollisionManager.h"
#include "ComponentSystem/EntityManager.h"
#include "Components.h"
#include "EnemySpawner.h"
#include "EntityFactory.h"
#include "GameClock.h"
#include "GlobalGameSettings.h"
#include "HUDManager.h"
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
	const float ftSlice { 16.f }; //The time slice length we want to update our game logic one time in. (milli)
	const float ftStep { 16.f };  //The time actually passed to the game logic, making sure we have constant result. (milli)
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
	HUDManager* hudManager { nullptr };

	GameClock* gameClock { nullptr };
	float currentSpawnCount { 5 };
	bool spawnLock { false };
	EnemySpawnMode currentWaveMode { EnemySpawnMode::Easy };

	//TO BE DONE IN A AUDIO CONTROLLER
	sf::Music bgm;
	sf::Sound sound;

	void Init();
	void InitLevel();
	void InitPlayer();
	void InitEnemy();

	void GenerateLevel();
	void GenerateEnemyWave();

	void ClearStage();
	void PauseStage();

	void PollingEvent();
	void OnGameStateChange(EventNames state);

public:
	eventpp::EventDispatcher<int, void(const MyEvent&), MyEventPolicies> gameDispatcher;
	GameStates GameState;

	Game();
	virtual ~Game();

	//Functions
	void Run();
	void FixedUpdate();
	void Update();
	void Render();
};