#pragma once
#include "GlobalPath.h"

//Basic Game
constexpr float ScreenWidth = 800.0f;
constexpr float ScreenHeight = 600.0f;
constexpr int EnemyBaseHealth = 1.f;
constexpr float PlayerBaseSpeed = 160.f;
constexpr float EnemyBaseSpeed = 120.f;
constexpr float EnemyRotateSpeed = 5.f;

constexpr float BulletBaseSpeed = 400.f;
constexpr float HitCoolDown = 0.1f;
constexpr int HurtPenalty = -50;

//Update Method
constexpr bool UseDeltaTime { true };

//Clock
constexpr float DefaultTimeLimit = 120.f;

//Wave
constexpr int WaveInterval = 10;
constexpr int WaveModeInterval = WaveInterval * 3;
constexpr int EliteInterval = (DefaultTimeLimit / WaveInterval) * 0.5f * WaveInterval;
constexpr int WaveBaseSpawn = 5;
constexpr int WaveSpawnOffset = 2;

//Game States
enum GameStates : std::size_t
{
	Menu,
	Stage,
	Result
};

//Event content
struct MyEvent
{
	int type;
	std::string message;
	int param;
};
struct MyEventPolicies
{
	static int getEvent(const MyEvent& e)
	{
		return e.type;
	}
};

//Events Name
enum EventNames : int
{
	GameStart,
	Win,
	GameOver,
	Restart,
	ScoreChange,
	PlayerHPChange
};

//Enums
enum EntityGroup : std::size_t
{
	Player,
	Enemy,
	Obstacle,
	Projectile,
	Weapon,
	Consumable
};

enum WeaponType
{
	Gun,
	Knife,
	Orbital
};

enum EnemyMoveType
{
	ChasePlayer,
	AvoidPlayer,
	PingPong,
	Charger
};

enum EnemySpawnMode : std::size_t
{
	Easy,
	Normal,
	Hard,
	VeryHard
};