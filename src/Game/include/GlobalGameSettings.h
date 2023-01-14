#pragma once
#include "GlobalPath.h"

//Window
const float ScreenWidth = 800.0f;
const float ScreenHeight = 600.0f;
const float PlayerBaseSpeed = 0.35f;
const float EnemyBaseSpeed = 0.1f;
const float BulletBaseSpeed = 1.2f;

//Clock
const float DefaultTimeLimit = 2.f;

//Wave
const int WaveInterval = 10.f;
const int WaveModeInterval = WaveInterval * 2;
const int WaveCountOffset = 1;

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

enum EnemySpawnMode
{
	Easy,
	Normal,
	Hard
};