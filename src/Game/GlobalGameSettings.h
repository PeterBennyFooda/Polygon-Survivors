#pragma once

//Window
const float ScreentWidth = 800.0f;
const float ScreenHeight = 600.0f;
const float PlayerBaseSpeed = 0.35f;
const float EnemyBaseSpeed = 0.1f;
const float BulletBaseSpeed = 0.2f;

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