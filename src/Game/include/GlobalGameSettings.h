#pragma once
#include "GlobalPath.h"

//Basic Game
constexpr float ScreenWidth = 800.0f;
constexpr float ScreenHeight = 600.0f;
constexpr float PlayerBaseSpeed = 0.3f;
constexpr float EnemyBaseSpeed = 0.1f;
constexpr float BulletBaseSpeed = 0.55f;
constexpr float HitCoolDown = 0.45f;

//Update Method
constexpr bool UseDeltaTime { true };

//Clock
constexpr float DefaultTimeLimit = 300.f;

//Wave
constexpr int WaveInterval = 10;
constexpr int WaveModeInterval = WaveInterval * 2;
constexpr int WaveSpawnOffset = 1;

//Game States
enum GameStates : std::size_t
{
	Menu,
	Stage,
	Result
};

//Events Name
enum EventNames : std::size_t
{
	GameStart,
	Win,
	GameOver,
	Restart
};

//Enums
enum EntityGroup : std::size_t
{
	Player,
	Enemy,
	Obstacle,
	Projectile,
	Weapon
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
	PingPong
};

enum EnemySpawnMode : std::size_t
{
	Easy,
	Normal,
	Hard
};